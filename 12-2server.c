#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include "server.h"

typedef struct sockaddr SA;

int Info_rmark = 0;//文件info.dat互斥操作的读
int Info_wmark = 0;//文件info.dat互斥操作的写
int User_rmark = 0;//文件User.dat互斥操作的读
int User_wmark = 0;//文件User.dat互斥操作的写

/******************************************************
 * 线程处理函数，每登陆一个用户，则创建一个线程用来处理
 *****************************************************/
void *handler(void *arg){
	int confd = *((int *)arg);
	MSG msg;
	int n;

	while(1){
		/*接收客户端发送信息结构体*/
		n = recv(confd, &msg, sizeof(MSG), 0);
		printf("get message from %s type:%d sign:%d\n",\
				msg.name, msg.type, msg.sign);

		/*如果收到退出宏，则线程退出，程序自动回到接收请求处*/
		if(msg.type == QUIT){
			printf("user %s quit\n", msg.name);
			pthread_exit(NULL);
			close(confd);
		}
		
		if(n == -1){
			break;
		}

		/*执行核心函数，分析客户端请求，执行相应操作*/
		GetMsg(&msg);
		printf("send message to %s type:%d sign:%d\n\n", 
				msg.name, msg.type, msg.sign);
		/*发送执行结果给客户端*/
		send(confd, &msg, sizeof(MSG), 0);
	}

	close(confd);
	pthread_exit(NULL);

}
/*服务器端主函数*/
int main(int argc, const char *argv[])
{
	/*************************************************
	 *将管理员的登陆信息先写入到文件user.dat中
	 *后续登陆时将读取文件进行判断，保证管理员登陆成功
	 ************************************************/
#if 1
	USER user;
	strcpy(user.name, "admin");
	strcpy(user.passwd, "123");

	user.type = 11;
	user.no = 111111;

	FILE *fp = fopen("./user.dat", "wb");
	fwrite(&(user), sizeof(USER), 1, fp);

	fclose(fp);
#endif

	int listenfd, confd;

	struct sockaddr_in serveraddr, clientaddr;

	/*命令行传参判断，是否符合传参规则*/
	if(argc != 3){
		printf("please input %s <ip> <port>\n", argv[0]);
		return -1;
	}

	/*创建流式套接字*/
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket error");
		return -1;
	}

	/*填充网络信息结构体*/
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	/*绑定套接字*/
	if(bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) == -1){
		perror("bind error");
		return -1;
	}

	/*监听客户端连接*/
	listen(listenfd, 5);

	bzero(&clientaddr, sizeof(clientaddr));
	socklen_t len = sizeof(clientaddr);
	printf("listenfd = %d\n", listenfd);

	while(1){
		/*等待客户端连接请求*/
		if((confd = accept(listenfd, (SA *)&clientaddr, &len)) == -1){
			perror("accept error");
			return -1;
		}

		/*输出客户端地址以及端口信息*/
		printf("connect with ip: %s, port: %d\n",\ 
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		/*服务器收到客户端请求之后，创建线程与客户端进行通信*/
		pthread_t thread;

		if(pthread_create(&thread, NULL, handler, (void *)&confd) < 0){
			perror("pthread_create error");
			return -1;
		}
	}

	close(listenfd);
	return 0;
}
int GetMsg(MSG *msg){
	switch(msg->type){
		case READ:
			FindMsg(msg);
			break;
		case CHANGE:
			DelUser(msg);
			DelMsg(msg);
			AddUser(msg);
			AddMsg(msg);
			break;
		case ADD:
			AddUser(msg);
			AddMsg(msg);
			break;
		case DELETE:
			DelUser(msg);
			DelMsg(msg);
			break;
		case LOAD:
			FindUser(msg);
			break;
		default:
			break;
	}
	return 0;
}

void FindUser(MSG *msg){
	FILE *fp;
	int flag = 0;

	/**************************************************
	 *实现对文件读写的互斥，如果写user.dat文件的写标志位大于0，
	 *表示此时有线程在对文件进行写操作，此时执行循环判断，直到
	 *标志位为0，则结束循环。
	 *************************************************/
	while(User_wmark > 0){
		usleep(100000);
	}

	/********************************************************
	 * 在执行读操作之前，将对文件user.dat的读标志位进行自加，
	 *其他任务在操作之前则判断此标志位，如果大于0,则不允许操作
	 ********************************************************/
	User_rmark++;

	/*打开存放用户登陆信息的文件*/
	if((fp = fopen("./user.dat", "rb")) == NULL){
		printf("User %s request:no file user.dat\n", msg->name);
		msg->sign = FAILED;
		strcpy(msg->data, "no file");
		return;
	}

	USER user_temp;

	/*读取文件中的信息，将存放信息的结构体USER依次取出*/
	while(fread(&user_temp, sizeof(USER), 1, fp) != 0){
		/*将文件中读取出的结构体信息中的名字信息与客户端请求的
		 *结构体中的名字进行对比，判断是否有一致的名字*/
		if(strcmp(user_temp.name, msg->name) == 0){
			/*如果名字一致，则继续判断登陆密码*/
			if(strcmp(user_temp.passwd, msg->passwd) == 0){
				/*满足以上条件，则判断登陆成功，设置对应的标志位*/
				flag = 1;
				msg->sign = SUCCESS;
				msg->info.type = user_temp.type;
				strcpy(msg->data, "all is right");
				return;
			}
		}
	}
	/*如果flag没有变化，说明未找到匹配信息，设置对应的标志位*/
	if(flag == 0){
		msg->sign = FAILED;
		strcpy(msg->data, "find user failed\n");
		return;
	}

	fclose(fp);
	/*操作完成，标志位恢复*/
	User_rmark--;
}
/***********************************************
 *向保存用户登陆信息的文件中，添加新用户的信息
 *其目的是保证后续该用户可以登陆系统
 ***********************************************/
void AddUser(MSG *msg){
	FILE *fp;
	USER user;
	/*读取客户端发送的结构体MSG，保存的是新用户的信息
	 *并将其复制到结构体USER中，之后写入文件中*/
	strcpy(user.name, msg->info.name);
	strcpy(user.passwd, msg->passwd);

	user.type = STAFF;
	user.no = msg->info.no;

	/********************************************
	 *判断对user.dat文件的读写标志位是否为0
	 *如果大于0,表示当前有线程在操作此文件
	 *******************************************/
	while((User_wmark > 0) && (User_rmark > 0)){
		usleep(100000);
	}

	/*在将新用户信息写入文件之前，操作标志位自加对其他线程执行互斥*/
	User_wmark++;

	/*打开存放用户登陆信息的文件*/
	if((fp = fopen("./user.dat", "ab")) == NULL){
		printf("User %s request: open user.dat failed\n", msg->name);
		msg->sign = FAILED;
		strcpy(msg->data, "no file");
		return;
	}

	/*写入新用户的登陆信息*/
	fwrite(&(user), sizeof(USER), 1, fp);
	printf("add user for %s ok\n", msg->name);

	/*写入成功，发送写入成功标志*/
	msg->sign = SUCCESS;
	strcpy(msg->data, "add user ok\n");

	fclose(fp);
	/*恢复标志位*/
	User_wmark--;
}
/*添加用户的详细信息*/
void AddMsg(MSG *msg){
	FILE *fp;

	/**************************************************
	 *判断info.dat文件的读写标志位，如果标志位大于0
	 *表示此时文件正在被其他线程操作，则执行循环等待
	 **************************************************/
	while((Info_wmark > 0) && (Info_rmark > 0)){
		usleep(100000);
	}

	/**************************************************
	 *在将新用户的详细信息写入文件之前，
	 *对info.dat文件的读写标志位进行自加，
	 *对其他线程实现互斥
	 *************************************************/
	Info_wmark++;

	/*打开用于存储用户详细信息的文件*/
	if((fp = fopen("./info.dat", "ab")) == NULL){
		printf("User %s request:open info.dat failed\n", msg->name);
		msg->sign = FAILED;
		strcpy(msg->data, "no file");
		return;
	}

	/*将新用户的详细信息写入到文件中保存*/
	fwrite(&(msg->info), sizeof(INFO), 1, fp);

	/*写入成功，设置对应的标志位，之后发送给客户端*/
	printf("add info for %s ok\n", msg->name);
	msg->sign = SUCCESS;
	strcpy(msg->data, "write info ok\n");

	fclose(fp);
	/*恢复标志位*/
	Info_wmark--;
}
void DelUser(MSG *msg){
	FILE *fp;
	int i = 0;
	USER user_temp[N];

	/*************************************************
	 *检测文件user.dat文件的写标志位，由于读操作不会
	 *对文件中的内容产生影响，因此不需要关注读标志位
	 *如果标志位大于0,则执行循环等待
	 *************************************************/
	while(User_wmark > 0){
		usleep(100000);
	}
	
	/*对文件user.dat的读标志位进行自加，实现互斥*/
	User_rmark++;

	/*打开存放用户登陆信息的文件*/
	if((fp = fopen("./user.dat", "rb")) == NULL){
		printf("User %s request:open user.dat failed\n", msg->name);
		msg->sign = FAILED;
		strcpy(msg->data, "no file");
		return;
	}

	/*将文件中所有的业主信息全部取出，保存到结构体数组中*/
	while(fread(&(user_temp[i++]), sizeof(USER), 1, fp) != 0){
		;
	}

	fclose(fp);
 	User_rmark--;

	/*判断读写标志位，原理同上*/
	while((User_rmark > 0) && (User_wmark > 0)){
		usleep(100000);
	}

	User_wmark++;

	/*重新打开文件，并清空文件中原有的数据*/
	if((fp = fopen("./user.dat", "wb")) == NULL){
		printf("User %s request:open user.dat failed\n", msg->name);
		msg->sign = FAILED;
		strcpy(msg->data, "no file");
		return;
	}

	/********************************************************
	 *判断需要删除的用户的编号与文件中所有的用户的编号进行对比
	 *由于编号设定时是唯一的，因此将判断符合条件的结构体从数组中
	 *移除，并将移除之后的数组中的结构体依次重新写入文件，即实现删除
	 ********************************************************/
	while(i--){
		if(msg->info.no != user_temp[i].no){
			fwrite(&(user_temp[i]), sizeof(USER), 1, fp);
		}
	}

	/*操作完成后，设置对应的标志位*/
	msg->sign = SUCCESS;

	printf("delete user for %s ok\n", msg->name);
	strcpy(msg->data, "delete user ok\n");

	fclose(fp);
	User_wmark--;
}
/*删除业主的详细信息，其原理与删除登录信息时一致*/
void DelMsg(MSG *msg){
	FILE *fp;
	int i = 0;
	INFO info_temp[N];

	/*实现对文件读写的互斥*/
	while(Info_wmark > 0){
		usleep(100000);
	}

	Info_rmark ++;

	/*打开用于存储用户详细信息的文件*/
	if((fp = fopen("./info.dat", "rb")) == NULL){
		printf("User %s request:open info.dat failed\n", msg->name);
		msg->sign = FAILED;
		strcpy(msg->data, "no file");
		return;
	}

	/*将存储用户详细信息的结构体依次从文件中读出，并保存在结构体数组中*/
	while(fread(&(info_temp[i++]), sizeof(INFO), 1, fp) != 0){
		;
	}

	fclose(fp);
	Info_rmark--;

	while((Info_rmark > 0) && (Info_wmark > 0)){
		usleep(100000);
	}

	Info_wmark++;

	/*重新打开文件*/
	if((fp = fopen("./info.dat", "wb")) == NULL){
		printf("User %s request:open info.dat failed\n", msg->name);
		msg->sign = FAILED;
		strcpy(msg->data, "no file");
		return;
	}

	/*******************************************************
	 *将结构体数组中的保存用户详细信息的结构体重新写入文件
	 *写入时，将符合删除条件的结构体跳过写入操作，实现删除 
	 ******************************************************/
	while(i--){
		if(msg->info.no == info_temp[i].no)
			continue;
		fwrite(&(info_temp[i]), sizeof(INFO), 1, fp);
	}

	printf("delete info for %s ok\n", msg->name);
	msg->sign = SUCCESS;
	strcpy(msg->data, "change info ok\n");

	fclose(fp);
	Info_wmark--;
}
/*实现查询业主信息*/
void FindMsg(MSG *msg){
	INFO info_temp;
	int flag = 0;
	FILE *fp;

	while(Info_wmark >0){
		usleep(100000);
	}

	Info_rmark++;

	/*打开存储业主详细信息的文件*/
	if((fp = fopen("./info.dat", "rb")) == NULL){
		printf("User %s request:no file info.dat\n", msg->name);
		msg->sign = FAILED;
		strcpy(msg->data, "no file");
		return;
	}

	if(strcmp(msg->info.name, "NULL") != 0){
		/*从文件中依次读取描述业主信息的结构体*/
		while(fread(&info_temp, sizeof(INFO), 1, fp) != 0){
			/*判断客户端需要查询的业主姓名与文件中读取的是否一致*/
			if(strcmp(info_temp.name, msg->info.name) == 0){
				/*如果名字相同，则再次判断其编号，避免业主名字相同，查询出错*/
				if((msg->info.no != 0) && (msg->info.no == info_temp.no)){
					/*判断符合条件则将该结构体之后发送给客户端*/
					msg->info = info_temp;
					msg->sign = SUCCESS;
					strcpy(msg->data, "find it2");
					flag = 1;
					return;
				}
				else{
					continue;
				}
			}
		}
		if(flag == 0){
			msg->sign = FAILED;
			strcpy(msg->data, "not find");
			return;
		}
	}

	fclose(fp);
	Info_rmark--;
}

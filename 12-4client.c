#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "client.h"

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;

	MSG msg;

	/*创建流式套接字*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket error");
		return -1;
	}

	/*填充网络信息结构体*/
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	/*连接服务器*/
	if(connect(sockfd, (struct sockaddr *)&serveraddr,\ 
				sizeof(serveraddr)) < 0){
		perror("connect error");
		return -1;
	}

	/*登录界面，使用循环，保证操作错误时可以再次返回该界面*/
	while(1){
		puts("==============================================");
		puts("+++++++++++++++++++Login++++++++++++++++++++++");
		puts("==============================================");

		/*输入登录信息 名字+密码*/
		printf("Please input your name >");
		fgets(msg.name, N, stdin);
		msg.name[strlen(msg.name) -1] = '\0';

		printf("Please input your password >");
		fgets(msg.passwd, N, stdin);
		msg.passwd[strlen(msg.passwd) - 1] = '\0';
		msg.type = LOAD;

		/*发送消息给服务器，进行登陆验证*/
		send(sockfd, &msg, sizeof(MSG), 0);
		printf("---load type %d\n", msg.type);
		/*接收服务器的反馈消息*/
		recv(sockfd, &msg, sizeof(MSG), 0);
		
		/*根据服务器端返回的标志进行判断*/
		/*登陆成功*/
		if(msg.sign == SUCCESS){
			/*进入业主登陆界面*/
			if(msg.info.type == STAFF){
				goto User;
			}
			/*进入物业管理界面*/
			else if(msg.info.type == ADM){
				goto Admin;
			}
		}
		/*登陆失败*/
		if(msg.sign == FAILED){
			printf("%s\n", msg.data);
			continue;
		}
	}
/*跳转到物业管理界面*/
Admin:
 	while(1){
		/*管理员权限*/
		puts("===========================================================");
		puts("1:add user 2:delete user 3:modify info 4:select info 5:exit");
		puts("===========================================================");
		printf("please input your command > ");//输入对应的操作数字

		/*输入错误命令情况处理*/
		int result;
		int command;
		char clear[N];

		if(scanf("%d", &command) == 0){
			fgets(clear, N, stdin);
			continue;
		}

		switch(command){
			case 1:
				/*添加业主信息*/
				result = do_adduser(sockfd, &msg);
				if(result == SUCCESS){
					puts("注册业主信息成功");
				}
				else if(result == FAILED){
					printf("%s\n", msg.data);
					continue;
				}
				break;
			case 2:
				/*删除业主信息*/
				result = do_deluser(sockfd, &msg);
				if(result == SUCCESS){
					puts("删除业主信息成功");
				}
				else if(result == FAILED){
					printf("%s\n", msg.data);
					puts("删除业主信息失败");
					continue;
				}
				break;
			case 3:
				/*修改业主信息*/
				result = do_modifyuser(sockfd, &msg);
				if(result == SUCCESS){
					puts("修改业主信息成功");
				}
				else if(result == FAILED){
					printf("%s\n", msg.data);
					puts("修改业主信息失败");
					continue;
				}
				break;
			case 4:
				/*查询业主信息*/
				result = do_selectuser(sockfd, &msg);
				if(result == SUCCESS){
					printf("姓名:%s\n", msg.info.name);
					printf("业主详细地址:%s\n", msg.info.addr);
					printf("停车费年卡办理时间:%s\n", msg.info.time_start);
					printf("停车卡有效截止日期:%s\n", msg.info.time_end);
					printf("车位编号:%d\n", msg.info.location);
					printf("业主编号:%d\n", msg.info.no);
					printf("年卡费用:%d\n", msg.info.card);
					printf("手机号:%s\n", msg.info.phone);
					printf("用户类型:%d\n", msg.info.type);
					printf("车牌号:%s\n", msg.info.car_num);
				}
				else if(result == FAILED){
					printf("%s\n", msg.data);
					puts("业主信息不存在");
					continue;
				}
				break;
			case 5:
				msg.type = QUIT;
				send(sockfd, &msg, sizeof(MSG), 0);
				goto Exit;
		}
	}

/*跳转到业主界面*/
User:
	while(1){
		/*普通业主权限*/
		puts("==============================================");
		puts("+++++1:select info 2:modify passwd 3:exit+++++");
		puts("==============================================");
		printf("please input your command > ");

		/*处理输入错误命令的情况*/
		int command;
		char clear[N];
		/****************************************************
		 * 如果终端输入的内容未被成功读取，则返回值为零;
		 * 说明本次输入的选择，不合规则，无法读取;
		 * continue跳过本次循环，重新让业主选择
		 ***************************************************/
		if(scanf("%d", &command) == 0){
			fgets(clear, N, stdin);
			continue;
		}
		
		switch(command){
			case 1:
				msg.type = READ;
				strcpy(msg.info.name, msg.name);
				printf("请输入编号 > ");
				input_no:
				if(scanf("%d", &(msg.info.no)) == 0){
					printf("input type error, exp 1001\n");
					fgets(clear, N, stdin);
					goto input_no;
				}
				/*发送查询消息*/
				send(sockfd, &msg, sizeof(MSG), 0);
				/*接收服务器的反馈消息*/
				recv(sockfd, &msg, sizeof(MSG), 0);
				/*打印用户自身消息*/
				printf("姓名:%s\n", msg.info.name);
				printf("业主详细地址:%s\n", msg.info.addr);
				printf("办停车卡时间:%s\n", msg.info.time_start);
				printf("停车卡有效期:%s\n", msg.info.time_end);
				printf("车位编号:%d\n", msg.info.location);
				printf("业主编号:%d\n", msg.info.no);
				printf("年卡费用:%d\n", msg.info.card);
				printf("手机号:%s\n", msg.info.phone);
				printf("用户类型:%d\n", msg.info.type);
				printf("车牌号:%s\n", msg.info.car_num);
				break;
			case 2:
				/*向服务器端确认需要修改密码的用户的编号以及名字*/
				strcpy(msg.info.name, msg.name);
				getchar();
				printf("请输入业主编号 > ");
				
				input_no1:
				if(scanf("%d", &(msg.info.no)) == 0){
					printf("input type error, exp 1001\n");
					fgets(clear, N, stdin);
					goto input_no1;
				}
				getchar();
				printf("请输入新的登录密码 > ");
				fgets(msg.passwd, N, stdin);
				msg.passwd[strlen(msg.passwd) - 1] = '\0';
				
				msg.type = CHANGE;
				send(sockfd, &msg, sizeof(MSG), 0);
				break;
			case 3:
				msg.type = QUIT;
				send(sockfd, &msg, sizeof(MSG), 0);
				goto Exit;
		}
	}
Exit:
	close(sockfd);
	return 0;
}

/*添加业主信息*/
int do_adduser(int sockfd, MSG *msg){
	printf("请输入业主的姓名 > ");
	getchar();
	fgets((msg->info).name, N, stdin);
	(msg->info).name[strlen((msg->info).name) - 1] = '\0';

	printf("请输入业主的详细地址 > ");
	fgets((msg->info).addr, N, stdin);
	(msg->info).addr[strlen((msg->info).addr) - 1] = '\0';

	printf("请输入业主的手机号码 > ");
	fgets((msg->info).phone, N, stdin);
	(msg->info).phone[strlen((msg->info).phone) - 1] = '\0';

	printf("请输入业主的车牌号 > ");
	fgets((msg->info).car_num, N, stdin);
	(msg->info).car_num[strlen((msg->info).car_num) - 1] = '\0';

	printf("请输入业主办理停车位起始日期 > ");
	fgets((msg->info).time_start, N, stdin);
	(msg->info).time_start[strlen((msg->info).time_start) - 1] = '\0';

	printf("请输入业主停车位使用截止日期 > ");
	fgets((msg->info).time_end, N, stdin);
	(msg->info).time_end[strlen((msg->info).time_end) - 1] = '\0'; 
	
	char clear[N];

input_location:
	printf("请输入业主停车位编号 > ");
	if(scanf("%d", &(msg->info.location)) == 0){
		printf("input type error, exp 1001\n");
		fgets(clear, N, stdin);
		goto input_location;
	}
	getchar();
input_no:
	printf("请输入业主编号 > ");
	if(scanf("%d", &(msg->info.no)) == 0){
		printf("input type error, exp 1001\n");
		fgets(clear, N, stdin);
		goto input_no;
	}
	getchar();
input_card:
	printf("请输入停车位年卡费用 > ");
	if(scanf("%d", &(msg->info.card)) == 0){
		printf("input type error, exp 1200\n");
		fgets(clear, N, stdin);
		goto input_card;
	}
	getchar();
input_type:
	printf("请输入业主类型 > ");
	if(scanf("%d", &(msg->info.type)) == 0){
		printf("input type error\n");
		printf("类型选择：1.物业管理 2.小区常住业主 3.小区租户\n");
		fgets(clear, N, stdin);
		goto input_card;
	}
	getchar();

	printf("请输入业主系统登陆密码 > ");
	fgets(msg->passwd, N, stdin);
	msg->passwd[strlen(msg->passwd) - 1] = '\0'; 	
	
	/*发送操作类型给服务器*/
	msg->type = ADD;

	/*发送给服务器的结构体类型，必须和客户端一致*/
	send(sockfd, msg, sizeof(MSG), 0);
	recv(sockfd, msg, sizeof(MSG), 0);

	return msg->sign;//返回服务器端的处理信息
}

/*删除业主信息*/
int do_deluser(int sockfd, MSG *msg){
	printf("请输入删除业主的姓名 > ");
	getchar();
	fgets((msg->info).name, N, stdin);
	(msg->info).name[strlen((msg->info).name) - 1] = '\0';

	printf("请输入删除业主的编号 > ");
	if(scanf("%d", &(msg->info.no)) == 0){
		msg->info.no = 0;
	}

	msg->type = DELETE;

	send(sockfd, msg, sizeof(MSG), 0);
	recv(sockfd, msg, sizeof(MSG), 0);

	return msg->sign;//返回服务器端的处理信息
}
/*修改业主信息*/
int do_modifyuser(int sockfd, MSG *msg){
	char clear[N];
	
	printf("请输入被修改业主的姓名 > ");
	getchar();
	fgets((msg->info).name, N, stdin);
	(msg->info).name[strlen((msg->info).name) - 1] = '\0';

input_no:
	printf("请输入被修改业主编号 > ");
	if(scanf("%d", &(msg->info.no)) == 0){
		printf("input type error, exp 1001\n");
		fgets(clear, N, stdin);
		goto input_no;
	}
	getchar();

	printf("请输入业主新的详细地址 > ");
	fgets((msg->info).addr, N, stdin);
	(msg->info).addr[strlen((msg->info).addr) - 1] = '\0';

	printf("请输入业主新的手机号码 > ");
	fgets((msg->info).phone, N, stdin);
	(msg->info).phone[strlen((msg->info).phone) - 1] = '\0';

	printf("请输入业主新的车牌号 > ");
	fgets((msg->info).car_num, N, stdin);
	(msg->info).car_num[strlen((msg->info).car_num) - 1] = '\0';

	printf("请输入业主新办理停车位起始日期 > ");
	fgets((msg->info).time_start, N, stdin);
	(msg->info).time_start[strlen((msg->info).time_start) - 1] = '\0';

	printf("请输入业主新停车位使用截止日期 > ");
	fgets((msg->info).time_end, N, stdin);
	(msg->info).time_end[strlen((msg->info).time_end) - 1] = '\0'; 
	

input_location:
	printf("请输入业主新停车位编号 > ");
	if(scanf("%d", &(msg->info.location)) == 0){
		printf("input type error, exp 1001\n");
		fgets(clear, N, stdin);
		goto input_location;
	}
	getchar();

input_card:
	printf("请输入停车位年卡费用 > ");
	if(scanf("%d", &(msg->info.card)) == 0){
		printf("input type error, exp 1200\n");
		fgets(clear, N, stdin);
		goto input_card;
	}
	getchar();
input_type:
	printf("请输入业主类型 > ");
	if(scanf("%d", &(msg->info.type)) == 0){
		printf("input type error, exp 2\n");
		fgets(clear, N, stdin);
		goto input_card;
	}
	getchar();

	printf("请输入业主系统登陆密码 > ");
	fgets(msg->passwd, N, stdin);
	msg->passwd[strlen(msg->passwd) - 1] = '\0'; 	
	
	/*发送操作类型给服务器*/
	msg->type = CHANGE;

	/*发送给服务器的结构体类型，必须和客户端一致*/
	send(sockfd, msg, sizeof(MSG), 0);
	recv(sockfd, msg, sizeof(MSG), 0);

	return msg->sign;//返回服务器端的处理信息
}
/*查询业主信息*/
int do_selectuser(int sockfd, MSG *msg){
	printf("请输入业主的姓名 > ");
	getchar();

	fgets((msg->info).name, N, stdin);
	(msg->info).name[strlen((msg->info).name) - 1] = '\0';

	/*当输入其他字符时，默认要查询的no值为0*/
	printf("请输入业主编号 > ");
	if(scanf("%d", &(msg->info.no)) == 0){
		msg->info.no = 0;
	}
	msg->type = READ;
	send(sockfd, msg, sizeof(MSG), 0);
	recv(sockfd, msg, sizeof(MSG), 0);

	return msg->sign;
}

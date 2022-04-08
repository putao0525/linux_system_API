#include <stdio.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/time.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s--%s--%d\n",\ 
								  __FILE__, __func__, __LINE__);\
						  exit(1);\
					     }while(0)

int main(int argc, const char *argv[])
{
	int sockfd, acceptfd;
	struct sockaddr_in serveraddr, clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	char buf[N] = {};

	/*初始化结构体*/
	bzero(&serveraddr, addrlen);
	bzero(&clientaddr, addrlen);

	if(argc < 3)
	{
		fprintf(stderr, "Usage: %s ip port\n", argv[0]);
		exit(1);
	}

	/*第一步:创建套接字*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		errlog("fail to socket");
	}

	/*
	*第二步:填充服务器网络信息结构体
	*inet_addr:将点分十进制ip地址转化为网络字节序的整形数据
	*htons:将主机字节序转化为网络字节序
	*atoi:将数字形字符串转化为整形数据
	*/
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	/*第三步:将套接字与服务器网络信息结构体绑定*/
	if(bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
	{
		errlog("fail to bind");
	}

	//第四步:将套接字设置为被动监听模式
	if(listen(sockfd, 5) < 0)
	{
		errlog("fail to listen");
	}

	/*
	*使用select函数实现io多路复用
	*注意:当select函数运行结束之后,
	*会移除除当前准备就绪的文件描述符以外其他所有的
	*注意:由于会移除集合里面的文件描述符,所以需要每次都添加
	*/
	fd_set readfds;
	int maxfd, ret;
	
	/*第一步:清空集合*/
	FD_ZERO(&readfds);

	maxfd = sockfd;

	while(1)
	{
		/*第二步:将需要的文件描述符添加到集合里面*/
		FD_SET(0, &readfds);
		FD_SET(sockfd, &readfds);
		
		/*第三步:阻塞等待文件描述符准备就绪*/
		if((ret = select(maxfd + 1, &readfds, NULL, NULL, NULL)) < 0)
		{
			errlog("fail to select");
		}

		printf("ret = %d\n", ret);

		/*
		*第四步:判断是哪个文件描述符准备就绪并执行对应的io操作
		*由于select函数运行结束后,集合里面只剩下准备就绪的,
		*所以只要判断到底是那个文件描述符还在集合里面即可	
		*/
		if(FD_ISSET(0, &readfds) == 1)
		{
			fgets(buf, N, stdin);
			buf[strlen(buf) - 1] = '\0';
			
			printf("buf = %s\n", buf);
		}

		if(FD_ISSET(sockfd, &readfds) == 1)
		{
			if((acceptfd = accept(sockfd,\ 
							(struct sockaddr *)&clientaddr, &addrlen)) < 0)
			{
				errlog("fail to accept"); 
			}

			printf("ip: %s, port: %d\n",\ 
					inet_ntoa(clientaddr.sin_addr),\
					ntohs(clientaddr.sin_port));
		}
	}

	close(acceptfd);
	close(sockfd);
	
	return 0;
}

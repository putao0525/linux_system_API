#include <stdio.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <strings.h>
#include <sys/un.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s--%s--%d\n",\ 
								  __FILE__, __func__, __LINE__);\
						  exit(1);\
					     }while(0)

int main(int argc, const char *argv[])
{
	int sockfd, acceptfd;
	struct sockaddr_un serveraddr, clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	char buf[N] = {};

	//初始化结构体 memset bzero
	bzero(&serveraddr, addrlen);
	bzero(&clientaddr, addrlen);

	//第一步:创建套接字
	if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		errlog("fail to socket");
	}

	//第二步:填充服务器本地信息结构体
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, argv[1]);

	//第三步:将套接字与服务器网络信息结构体绑定
	if(bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0){
		errlog("fail to bind");
	}

	//第四步:将套接字设置为被动监听模式
	if(listen(sockfd, 5) < 0){
		errlog("fail to listen");
	}

	//第五步:阻塞等待客户端的连接请求
	if((acceptfd = accept(sockfd,\ 
					(struct sockaddr *)&clientaddr, &addrlen)) < 0){
		errlog("fail to accept");
	}

	ssize_t bytes;
	while(1){
		if((bytes = recv(acceptfd, buf, N, 0)) < 0){
			errlog("fail to recv");
		}
		else if(bytes == 0){
			printf("NO DATA\n");
			exit(1);
		}
		else{
			if(strncmp(buf, "quit", 4) == 0){
				printf("client quit\n");
				break;
			}
			else{
				printf("client: %s\n", buf);

				strcat(buf, " *_*");

				if(send(acceptfd, buf, N, 0) < 0){
					errlog("fail to send");
				}
			}
		}
	}

	close(acceptfd);
	close(sockfd);
	
	return 0;
}

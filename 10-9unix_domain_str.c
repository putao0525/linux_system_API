#include <stdio.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <sys/un.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s--%s--%d\n",\ 
								  __FILE__, __func__, __LINE__);\
						  exit(1);\
					     }while(0)

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_un serveraddr;
	socklen_t addrlen = sizeof(serveraddr);
	char buf[N] = {};

	//第一步:创建套接字
	if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		errlog("fail to socket");
	}

	//第二步:填充服务器本地信息结构体
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, argv[1]);

	//第三步:发送客户端连接请求
	if(connect(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0){
		errlog("fail to connect");
	}

	while(1){
		fgets(buf, N, stdin);
		buf[strlen(buf) - 1] = '\0';

		if(send(sockfd, buf, N, 0) < 0){
			errlog("fail to send");
		}

		if(strncmp(buf, "quit", 4) == 0){
			printf("client is quited\n");
			break;
		}
		else {
			if(recv(sockfd, buf, N, 0) < 0){
				errlog("fail to recv");
			}

			printf("server: %s\n", buf);
		}
	}

	close(sockfd);
	
	return 0;
}

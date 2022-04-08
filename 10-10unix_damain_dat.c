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
	struct sockaddr_un serveraddr, clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	char buf[N] = {};

	//第一步:创建套接字
	if((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0){
		errlog("fail to socket");
	}

	//第二步:填充服务器本地信息结构体
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, argv[1]);

	//第三步:将套接字与服务器网络信息结构体绑定
	if(bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0){
		errlog("fail to bind");
	}

	ssize_t bytes;
	while(1){
		if((bytes = recvfrom(sockfd, buf, N, 0,\ 
				(struct sockaddr *)&clientaddr, &addrlen)) < 0){
			errlog("fail to recvfrom");
		}
		else{
			printf("clientaddr.sun_path = %s\n", clientaddr.sun_path);

			if(strncmp(buf, "quit", 4) == 0){
				printf("client quit\n");
				break;
			}
			else{
				printf("client: %s\n", buf);

				strcat(buf, " *_*");

				if(sendto(sockfd, buf, N, 0,\ 
					(struct sockaddr *)&clientaddr, addrlen) < 0){
					errlog("fail to sendto");
				}
			}
		}
	}

	close(sockfd);
	
	return 0;
}

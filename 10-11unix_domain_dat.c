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
	if((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0){
		errlog("fail to socket");
	}

	//第二步:填充服务器本地信息结构体
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, argv[1]);

	//客户端需要绑定自己的信息,否则服务器无法给客户端发送数据
	struct sockaddr_un clientaddr;
	clientaddr.sun_family = AF_UNIX;
	strcpy(clientaddr.sun_path, argv[2]);

	if(bind(sockfd, (struct sockaddr *)&clientaddr, addrlen) < 0){
		errlog("fail to bind");
	}

	while(1){
		fgets(buf, N, stdin);
		buf[strlen(buf) - 1] = '\0';

		if(sendto(sockfd, buf, N, 0,\ 
				(struct sockaddr *)&serveraddr, addrlen) < 0){
			errlog("fail to sendto");
		}

		if(strncmp(buf, "quit", 4) == 0){
			printf("client is quited\n");
			break;
		}
		else{
			if(recvfrom(sockfd, buf, N, 0,\ 
				(struct sockaddr *)&serveraddr, &addrlen) < 0){
				errlog("fail to recv");
			}
			printf("server: %s\n", buf);
		}
	}
	close(sockfd);
	
	return 0;
}

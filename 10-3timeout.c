#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
						  printf("---%s---%s---%d---\n",\
								  __FILE__, __func__, __LINE__);\
						  return -1;\
						 }while(0)
/*设置信号处理函数*/
void handler(int sig){
	printf("SIGALRM INTERRUPTED\n");
}
int main(int argc, const char *argv[])
{
	int sockfd;

	struct sockaddr_in serveraddr, clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	char buf[N] = "";
	struct sigaction act;
	if(argc < 3){
		fprintf(stderr, "Usage: %s ip port\n", argv[0]);
		return -1;
	}

	/*创建套接字*/
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		errlog("socket error");
	}

	/*填充网络信息结构体
	 *inet_addr:将点分十进制IP地址转换为网络字节序的整型数据
	 *htons:将主机字节序转换为网络字节序
	 *atoi:将数字型字符串转换为整型数据 
	 */
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	/*将套接字与服务器网络信息结构体绑定*/
	if(bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0){
		errlog("bind error");
	}
	/*消除之前的信号处理，设置当前的信号处理为空，类似于初始化*/
	sigaction(SIGALRM, NULL, &act);
	/*指定信号处理函数，信号到来时，执行该函数*/
	act.sa_handler = handler;
	/*清除自启动标志，即取消自启动*/
	act.sa_flags &= ~SA_RESTART;
	/*设置本次的信号处理*/
	sigaction(SIGALRM, &act, NULL);

	ssize_t bytes;

	while(1){
		/*设置闹钟，5s之后程序收到SIGALRM信号*/
		alarm(5);
		if((bytes = recvfrom(sockfd, buf, N, 0,\
					   (struct sockaddr *)&clientaddr, &addrlen)) < 0){
			errlog("recvfrom error");
		}
		else{
			printf("ip: %s, port: %d\n", 
					inet_ntoa(clientaddr.sin_addr), 
					ntohs(clientaddr.sin_port));

			if(strncmp(buf, "quit", 4) == 0){
				printf("server quit\n");
				break;
			}
			else{
				printf("client: %s\n", buf);

				strcat(buf, "-server");

				if(sendto(sockfd, buf, N, 0,\ 
							(struct sockaddr *)&clientaddr, addrlen) < 0){
					errlog("sendto error");
				}
			}
		}
		/*取消定时器信号*/
		alarm(0);
	}

	close(sockfd);
	return 0;
}


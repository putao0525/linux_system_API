#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
							printf("---%s---%s---%d---\n",\
							__FILE__, __func__, __LINE__);\
							return -1;\
						 }while(0)
void handler(int sig){
	/*通过信号使父进程执行等待回收子进程的资源的操作*/
	wait(NULL);
}
int main(int argc, const char *argv[])
{
	int sockfd, acceptfd;

	struct sockaddr_in serveraddr, clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	char buf[N] = "";
	pid_t pid;

	bzero(&serveraddr, addrlen);
	bzero(&clientaddr, addrlen);

	/*提示程序需要命令行传参*/
	if(argc < 3){
		fprintf(stderr, "Usage: %s ip port\n", argv[0]);
		return -1;
	}

	/*创建套接字*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		errlog("socket error");
	}

	/*填充网络信息结构体
	 *inet_addr：将电分十进制地址转换为网络字节序的整型数据
	 *htons：将主机字节序转换为网络字节序
	 *atoi：将数字型字符串转化为整型数据
	 */
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	/*将套接字与服务器网络信息结构体把绑定*/
	if(bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0){
		errlog("bind error");
	}

	/*将套接字设置为被动监听模式*/
	if(listen(sockfd, 5) < 0){
		errlog("listen error");
	}

	/*注册信号*/
	signal(SIGUSR1, handler);

	while(1){
		/*阻塞等待客户端的连接请求*/
		if((acceptfd = accept(sockfd,\ 
					(struct sockaddr *)&clientaddr, &addrlen)) < 0){
			errlog("accept error");
		}

		printf("ip: %s, port: %d\n",\
				inet_ntoa(clientaddr.sin_addr),\
				ntohs(clientaddr.sin_port));

		pid = fork();

		if(pid < 0){
			errlog("fork error");
		}
		else if(pid == 0){
			/*子进程专门负责处理客户端的消息，不负责客户端的连接*/

			/*释放资源，子进程不需要接收客户端的连接*/
			close(sockfd);
			while(1){
				ssize_t bytes;
				if((bytes = recv(acceptfd, buf, N, 0)) < 0){
					errlog("recv error");
				}
				else if(bytes == 0){
					errlog("no data\n");
				}
				else{
					if(strncmp(buf, "quit", 4) == 0){
						printf("client quit\n");
						sleep(1);
						break;
					}
					else{
						printf("client: %s\n", buf);
						strcat(buf, "-server");

						if(send(acceptfd, buf, N, 0) < 0){
							errlog("send error");
						}
					}
				}
			}
			kill(getppid(), SIGUSR1);
			exit(0);
		}
		else{
			/*
			 *父进程的执行代码段释放资源，不需要收发信息
			 *关闭收发信息的文件描述符
			 */
			close(acceptfd);
		}
	}
	return 0;
}

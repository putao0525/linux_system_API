#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
						  printf("---%s---%s---%d---\n",\
								  __FILE__, __func__, __LINE__);\
						  return -1;\
						 }while(0)
int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
	socklen_t addrlen = sizeof(serveraddr);
	char buf[N] = "";

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

#if 0
	系统可以随机为客户端指定IP地址和端口号，客户端也可以自己指定
	struct sockaddr_in clientaddr;
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr(argv[3]);
	clientaddr.sin_port = htons(atoi(argv[4]));

	if(bind(sockfd, (struct sockaddr *)&clientaddr, addrlen) < 0){
		errlog("bind error");
	}
#endif

	/*发送客户端连接请求*/
	if(connect(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0){
		errlog("connect error");
	}

	while(1){
		fgets(buf, N, stdin);
		buf[strlen(buf) - 1] = '\0';

		if(send(sockfd, buf, N, 0) < 0){
			errlog("send error");
		}
		else{
			if(strncmp(buf,"quit", 4) == 0){
				break;
			}

			if(recv(sockfd, buf, N, 0) < 0){
				errlog("recv error");
			}

			printf("server: %s\n", buf);
		}
	}

	return 0;
}

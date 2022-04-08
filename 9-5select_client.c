#include <stdio.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>

#define N 128
#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s--%s--%d\n",\ 
								  __FILE__, __func__, __LINE__);\
						  exit(1);\
					     }while(0)

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
	socklen_t addrlen = sizeof(serveraddr);

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

	/*系统可以随机为客户端指定ip地址和端口号,客户端自己也可以自己指定*/
#if 0
	struct sockaddr_in clientaddr;
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr(argv[3]);
	clientaddr.sin_port = htons(atoi(argv[4]));

	if(bind(sockfd, (struct sockaddr *)&clientaddr, addrlen) < 0)
	{
		errlog("fail to bind");
	}
#endif

	/*第三步:发送客户端连接请求*/
	if(connect(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
	{
		errlog("fail to connect");
	}

	close(sockfd);
	
	return 0;
}

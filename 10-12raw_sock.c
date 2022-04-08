#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <netinet/if_ether.h>

int main(int argc, const char *argv[])
{
	//创建原始套接字
	int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sockfd < 0)
	{
		perror("fail to socket");
		exit(1);
	}

	printf("sockfd = %d\n", sockfd);

	close(sockfd);
	
	return 0;
}


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

	//接收链路层数据
	int ret;
	unsigned short mac_type;
	while(1)
	{
		unsigned char buf[1600] = {};
		char src_mac[18] = {};
		char dest_mac[18] = {};

		ret = recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL);

		//从接收到的数据中获取源mac地址、目的mac地址以及类型
		sprintf(src_mac, "%x:%x:%x:%x:%x:%x",\ 
				buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
		sprintf(dest_mac, "%x:%x:%x:%x:%x:%x",\ 
				buf[0+6], buf[1+6], buf[2+6], buf[3+6], buf[4+6], buf[5+6]);

		mac_type = ntohs(*(unsigned short *)(buf + 12));

		printf("源mac：%s --> 目的mac：%s\n", src_mac, dest_mac);
		printf("type = %#x\n", mac_type);
		
		if(mac_type == 0x800)
		{
			printf("\n*******************IP数据包*********************\n");

			//定义指针变量保存IP数据包首地址
			unsigned char *ip_buf = buf + 14;
			
			//获取ip数据包首部长度
			//首部长度占4位，是数据包的前八位中的后四位，且单位是4字节
			int ip_headlen = ((*ip_buf) & 0x0f) * 4;
			printf("ip数据包首部长度： %d字节\n", ip_headlen);
			//获取IP封包的总长度，表示总长度的标志从IP数据包的第16位开始
			printf("ip数据包总长度：%d字节\n",\ 
					ntohs(*(unsigned short *)(ip_buf + 2)));

			//分析ip协议包中传输层协议(IP报文的上层协议)
			unsigned char ip_type = *(unsigned short *)(ip_buf + 9);
			if(ip_type == 1)
			{
				printf("ICMP协议\n");
			}
			else if(ip_type == 2)
			{
				printf("IGMP协议\n");
			}
			else if(ip_type == 6)
			{
				printf("TCP协议\n");
			}
			else if(ip_type == 17)
			{
				printf("UDP协议\n");
			}

			printf("************************************************\n\n");
		}
		else if(mac_type == 0x0806)
		{
			printf("\nARP数据包\n\n");
		}
		else if(mac_type == 0x8035)
		{
			printf("\nRARP数据包\n\n");
		}
	}

	close(sockfd);
	
	return 0;
}


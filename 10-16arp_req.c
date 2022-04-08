#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <netinet/if_ether.h> 
#include <netpacket/packet.h> 
#include <sys/ioctl.h> 
#include <net/if.h> 
#include <string.h> 

int main(int argc, const char *argv[])
{
	//创建原始套接字
	int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sockfd < 0){
		perror("fail to socket");
		exit(1);
	}

	//使用ARP请求获取10.0.36.250的mac地址 
	//MAC头 ARP头
	unsigned char msg[1600] = {
		//组mac头
		//目的mac,广播请求所使用的MAC地址
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		//源mac
		0x00, 0x0c, 0x29, 0xe6, 0x9e, 0x37,
		//帧类型
		0x08, 0x06,
		//硬件类型（以太网地址为1）
		0x00, 0x01,
		//协议类型
		0x08, 0x00,
		//硬件地址长度和协议地址长度（mac和ip）
		0x06, 0x04,
		//选项，ARP请求
		0x00, 0x01,
		//源mac
		0x00, 0x0c, 0x29, 0xe6, 0x9e, 0x37,
		//源ip地址
		10, 0, 36, 199,
		//目的mac
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		//目的ip
		10, 0, 36, 250	
	};

	//将arp请求报文发送出去，通过eth0发送出去
	//使用ioctl函数获取本机网络接口
	struct ifreq ethreq;
	strncpy(ethreq.ifr_name, "eth0", IFNAMSIZ);
	if(ioctl(sockfd, SIOCGIFINDEX, &ethreq) == -1){
		perror("fail to ioctl");
		exit(1);
	}

	//设置本机网络接口
	struct sockaddr_ll sll;
	bzero(&sll, sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;

	sendto(sockfd, msg, 42, 0, (struct sockaddr *)&sll, sizeof(sll));

	//接收数据
	while(1){
		unsigned char buf[1600] = {};

		recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL);
		unsigned short mactype = ntohs(*(unsigned short *)(buf + 12));
		//必须是arp数据包才分析
		if(mactype == 0x0806){
			//请求为arp应答
			unsigned short arpopt = htons(*(unsigned short *)(buf + 20));
			if(arpopt == 0x02){
				char mac[18] = {};
				sprintf(mac, "%x:%x:%x:%x:%x:%x", buf[0 + 6],\ 
						buf[1 + 6], buf[2 + 6], buf[3 + 6], buf[4 + 6], buf[5 + 6]);
				
				printf("10.0.36.250 --> %s\n", mac);
				break;
			}
		}
	}

	close(sockfd);
	return 0;
}


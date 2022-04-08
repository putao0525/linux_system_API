#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <netinet/if_ether.h> 
#include <netpacket/packet.h> //struct sockaddr_ll
#include <sys/ioctl.h> 
#include <net/if.h> //struct ifreq
#include <string.h>
#include <net/ethernet.h> //支持结构体struct ether_header
#include <net/if_arp.h> //支持结构体struct arphdr	

struct arphdr{
	unsigned short int ar_hrd;      
	unsigned short int ar_pro;      
	unsigned char ar_hln;      
	unsigned char ar_pln;      
	unsigned short int ar_op;   
#if 1
	unsigned char __ar_sha[ETH_ALEN];   
	unsigned char __ar_sip[4];      
	unsigned char __ar_tha[ETH_ALEN];
	unsigned char __ar_tip[4];   
#endif
	};

int main(int argc, const char *argv[])
{
	//创建原始套接字
	int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sockfd < 0){
		perror("fail to socket");
		exit(1);
	}

	//使用arp请求获取10.0.36.250的mac地址 

	unsigned char msg[1600] = {};
	unsigned char src_mac[6] = {0x00, 0x0c, 0x29, 0xe6, 0x9e, 0x37};
	struct ether_header *eth_head = (struct ether_header *)msg;
	//组mac头
	//目的mac
	memset(eth_head->ether_dhost, 0xff, 6);
	//源mac
	memcpy(eth_head->ether_shost, src_mac, 6);
	//协议类型
	eth_head->ether_type = htons(0x0806);

	//组arp头
	struct arphdr *arp_head = (struct arphdr *)(msg + 14);
	//硬件类型，以太网为1
	arp_head->ar_hrd = htons(1);
	//协议类型
	arp_head->ar_pro = htons(0x0800);
	//硬件地址长度
	arp_head->ar_hln = 6;
	//协议地址长度
	arp_head->ar_pln = 4;	
	//arp请求
	arp_head->ar_op = htons(1);
	//源mac
	memcpy(arp_head->__ar_sha, src_mac, 6);
	//源ip
	*(unsigned int *)(arp_head->__ar_sip) = inet_addr("10.0.36.199");
	//目的mac
	memset(arp_head->__ar_tha, 0, 6);
	//目的ip
	*(unsigned int *)(arp_head->__ar_tip) = inet_addr("10.0.36.250");

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
				sprintf(mac, "%x:%x:%x:%x:%x:%x",\
						buf[0 + 6], buf[1 + 6], buf[2 + 6],\
						buf[3 + 6], buf[4 + 6], buf[5 + 6]);
				
				printf("192.168.1.222 --> %s\n", mac);
				break;
			}
		}
	}

	close(sockfd);
	
	return 0;
}


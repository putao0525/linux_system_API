/*描述网络接口属性的结构体*/
struct ifreq ethreq;
/*将网卡的名称eth0拷贝到结构体成员ifr_name中，指定要获取属性的网卡*/
/*宏IFNAMSIZ表示拷贝的长度*/
strcpy(ethreq.ifr_ifrifr_name, “eth0”, IFNAMSIZ);
/*获得网卡为eth0接口信息,
 */*即对ethreq表示的结构体属性信息进行初始化，该信息一定与eth0有关系*/
if(ioctl(sock_raw_fd, SIOCGIFINDEX, &ethreq) == -1){
	perror(“ioctl error”);
	close(sock_raw_fd);
	return -1;
}
+return++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*将网络接口赋值给原始套接字地址结构*/
struct sockaddr_ll sll;
bzero(&sll, sizeof(sll));
sll.sll_sll_ifindex = ethreq.ifr_ifrifr_ifindex; /*获取本机的网络接口eth0*/
int len = sendto(sock_raw_fd, buf, sizeof(buf), 0,\ 
		(struct sockaddr *)&sll, sizeof(sll));


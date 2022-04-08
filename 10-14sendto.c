/*将网络接口赋值给原始套接字地址结构*/
 * struct sockaddr_ll sll;
 * bzero(&sll, sizeof(sll));
 * sll.sll_ifindex = /*获取本机的网络接口*/
int len = sendto(sock_raw_fd, buf, sizeof(buf), 0,\ 
		(struct sockaddr *)&sll, sizeof(sll));


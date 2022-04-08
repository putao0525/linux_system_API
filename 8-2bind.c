struct sockaddr_in serveraddr;

serveraddr.sin_family = AF_INET;
serveraddr.sin_addr.s_addrs_addr = inet_addr(“127.0.0.1”);
serveraddr.sin_port = htons(7777);

if(bind(sockfd, (struct sockadrr *)&serveraddr, sizeof(serveraddr)) < 0){
	perror(“bindind error”);
	return -1;
}
		

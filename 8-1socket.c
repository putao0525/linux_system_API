int sockfd;
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	perror(“fail to socket”);
	return -1;
}


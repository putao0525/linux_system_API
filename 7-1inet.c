#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, const char *argv[])
{
	int i;
	char lo[] = "127.0.0.1";
	struct in_addr netaddr;
	
	netaddr.s_addr = inet_addr(lo);
	printf("NetIP: 0x%x\n", netaddr.s_addr);
	
	char *straddr = inet_ntoa(netaddr);
	printf("StrIP: %s\n", straddr);
	
	int ret = inet_aton(straddr, &netaddr);
	printf("NetIP: 0x%x\n", netaddr.s_addr);
	return 0;
}



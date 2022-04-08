#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
int main(int argc, const char *argv[])
{
	struct in_addr addr;
	
	if(inet_pton(AF_INET, "127.0.0.1", &addr.s_addr) == 1)
		printf("NetIP: %x\n", addr.s_addr);
	
	char str[20];
	if(inet_ntop(AF_INET, &addr.s_addr, str, sizeof(str)))
		printf("StrIP: %s\n", str);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	printf("exit before...");
	exit(0);
	printf("exit after...");
	return 0;
}

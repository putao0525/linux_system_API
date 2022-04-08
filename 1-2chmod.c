#include <stdio.h>
#include <sys/stat.h>

int main(int argc, const char *argv[])
{
	if(chmod("test.txt", S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH) < 0){
		printf("chmod error\n");
		return -1;
	}
	return 0;
}

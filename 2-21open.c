#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, const char *argv[])
{
	int fd;

	if((fd = open("test.txt", O_WRONLY|O_CREAT|O_TRUNC, 0664)) > 0){
		printf("fd = %d\n", fd);		
	}
	close(fd);
	return 0;
}


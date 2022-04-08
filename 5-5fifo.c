#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define errlog(errmsg) do{perror(errmsg);\
	                      printf("--%s--%s--%d--\n",\ 
					              __FILE__, __FUNCTION__, __LINE__);\
					      return -1;}while(0)
int main(int argc, const char *argv[])
{
	int fd;	
	if(mkfifo("fifo", 0664) < 0){
		errlog("mkfifo error");
	}
	
	printf("open before\n");
	if((fd = open("fifo", O_RDWR)) > 0){
		printf("fd = %d\n", fd);
	}

	printf("open after\n");
	return 0;
}

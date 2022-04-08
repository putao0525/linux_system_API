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
		if(errno == EEXIST){
			fd = open("fifo", O_RDWR);
			printf("fd = %d\n", fd);
		}
		else{
			errlog("mkfifo error");
		}
	}
	else{
		fd = open("fifo", O_RDWR);
		printf("fd = %d\n", fd);
	}
	return 0;
}

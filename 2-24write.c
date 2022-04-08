#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "source.h"

#define errlog(errmsg) perror(errmsg);\
	                   printf("--%s--%s--%d--\n",\ 
					   __FILE__, __FUNCTION__, __LINE__);\
					   return -1;
struct data obj = {
	.a = 1,
	.b = 'w',
	.buf = "test",
};
int main(int argc, const char *argv[])
{
	int fd;

	if((fd = open("test.txt", O_WRONLY|O_CREAT|O_TRUNC, 0664)) < 0){
		errlog("open error");	
	}

	if(write(fd, &obj, sizeof(struct data)) < 0){
		errlog("write error");
	}
	close(fd);
	return 0;
}


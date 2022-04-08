#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "source.h"

#define errlog(errmsg) perror(errmsg);\
	                   printf("--%s--%s--%d--\n",\ 
					   __FILE__, __FUNCTION__, __LINE__);\
					   return -1;
int main(int argc, const char *argv[])
{
	int fd;
	struct data obj;
	ssize_t nbyte;
	if((fd = open("test.txt", O_RDONLY)) < 0){
		errlog("open error");	
	}

	if((nbyte = read(fd, &obj, sizeof(struct data))) > 0){
		printf("nbyte = %ld\n", nbyte);
		printf("a = %d b = %c buf = %s\n", obj.a, obj.b, obj.buf);
	}
	close(fd);
	return 0;
}


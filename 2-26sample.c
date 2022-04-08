#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define SRC_FILE "src_file"
#define DEST_FILE "dest_file"
#define OFFSET 10240
#define errlog(errmsg) perror(errmsg);\
	                   printf("--%s--%s--%d--\n",\ 
					   __FILE__, __FUNCTION__, __LINE__);\
					   return -1;
int main(int argc, const char *argv[])
{
	int fds, fdd;
	unsigned char buf[BUFFER_SIZE];
	ssize_t read_led;

	if((fds = open(SRC_FILE, O_RDONLY)) < 0){
		errlog("open error");	
	}

	if((fdd = open(DEST_FILE, O_WRONLY|O_CREAT|O_TRUNC, 0664)) < 0){
		errlog("open error");
	}

	lseek(fds, -OFFSET, SEEK_END);

	while((read_led = read(fds, buf, sizeof(buf))) > 0){
		write(fdd, buf, read_led);
	}

	close(fds);
	close(fdd);

	return 0;
}

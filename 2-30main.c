#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <stdlib.h>

int lock_set(int fd, int type);

int main(int argc, const char *argv[])
{
	int fd;

	if((fd = open("test.txt", O_RDWR)) < 0){
		perror("open error");
		return -1;
	}

	lock_set(fd, F_RDLCK);
	getchar();
	lock_set(fd, F_UNLCK);
	getchar();

	close(fd);
	return 0;
}


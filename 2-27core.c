#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define N 32
int main(int argc, const char *argv[])
{
	int fd;
	char buf1[N] = "abcdefg";
	char buf2[N] = "hijklmn";
	fd = open("test.txt", O_RDWR|O_CREAT|O_TRUNC, 0664);

	write(fd, buf1, strlen(buf1));

	lseek(fd, 65536, SEEK_CUR);

	write(fd, buf2, strlen(buf2));
	return 0;
}

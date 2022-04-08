#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

int main(int argc, const char *argv[])
{
	struct stat buf;
	struct utimbuf utimbuf;
	if(stat(argv[1], &buf) == -1){
		printf("stat error\n");
		return -1;
	}

	utimbuf.actime = buf.st_atime;
	utimbuf.modtime = buf.st_atime;

	if(utime(argv[2], &utimbuf) == -1){
		printf("utime error\n");
		return -1;
	}
	return 0;
}

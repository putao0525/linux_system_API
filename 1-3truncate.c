#include <stdio.h>
#include <sys/types.h>

int main(int argc, const char *argv[])
{
	off_t offset = 12;
	if(truncate(argv[1], offset) < 0){
		printf("truncate error\n");
		return -1;
	}
	return 0;
}

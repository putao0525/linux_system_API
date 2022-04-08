#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	if((fp = fopen("test.txt", "r")) == NULL){
		printf("fopen error:%s\n", strerror(errno));
		return -1;
	}

	fclose(fp);
	return 0;
}

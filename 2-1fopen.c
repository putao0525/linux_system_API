#include <stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	if((fp = fopen("test.txt", "w")) == NULL){
		printf("fopen error\n");
		return -1;
	}

	fclose(fp);
	return 0;
}

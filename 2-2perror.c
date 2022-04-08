#include <stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	if((fp = fopen("test.txt", "r")) == NULL){
		perror("fopen error");
		return -1;
	}

	fclose(fp);
	return 0;
}

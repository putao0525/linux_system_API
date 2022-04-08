#include <stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	if((fp = fopen("test.txt", "w")) == NULL){
		perror("fopen error");
		return -1;
	}

	if(fputc('a', fp) == EOF){
		perror("fputc error");
	}
	fclose(fp);
	return 0;
}

#include <stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	int ch;
	if((fp = fopen("test.txt", "r")) == NULL){
		perror("fopen error");
		return -1;
	}

	if((ch = fgetc(fp)) != EOF){
		printf("ch = %c\n", ch);
	}
	fclose(fp);
	return 0;
}

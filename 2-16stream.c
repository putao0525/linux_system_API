#include <stdio.h>
#define errlog(errmsg) perror(errmsg);\
	                   printf("--%s--%s--%d--\n",\ 
					   __FILE__, __FUNCTION__, __LINE__);\
					   return -1;
int main(int argc, const char *argv[])
{
	FILE *fp;
	int ch;
	long offset;
	if((fp = fopen("test.txt", "w+")) == NULL){
		errlog("fopen error");
	}

	fputc('a', fp);
	fputc('b', fp);
	fputc('c', fp);
	fputc('d', fp);

	while((ch = fgetc(fp)) != EOF){
		printf("ch = %c\n", ch);
	}
	
	fclose(fp);
	return 0;
}

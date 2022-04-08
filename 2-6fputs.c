#include <stdio.h>
#define N 32
#define errlog(errmsg) perror(errmsg);\
	                   printf("--%s--%s--%d--\n",\ 
					   __FILE__, __FUNCTION__, __LINE__);\
					   return -1;
int main(int argc, const char *argv[])
{
	FILE *fp;
	char buf[N] = "hello world";
	if((fp = fopen("test.txt", "w")) == NULL){
		errlog("fopen error");
	}

	if(fputs(buf, fp) == EOF){
		errlog("fputs error");
	}
	fclose(fp);
	return 0;
}

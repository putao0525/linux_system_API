#include <stdio.h>
#define N 32
#define errlog(errmsg) perror(errmsg);\
	                   printf("--%s--%s--%d--\n",\ 
					   __FILE__, __FUNCTION__, __LINE__);\
					   return -1;
int main(int argc, const char *argv[])
{
	FILE *fp;
	char buf[N] = "";
	if((fp = fopen("test.txt", "r")) == NULL){
		errlog("fopen error");
	}

	if(fgets(buf, N, fp) != NULL){
		printf("buf:%s\n", buf);
	}
	fclose(fp);
	return 0;
}

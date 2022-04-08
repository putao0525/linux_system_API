#include <stdio.h>
#define errlog(errmsg) perror(errmsg);\
	                   printf("--%s--%s--%d--\n",\ 
					   __FILE__, __FUNCTION__, __LINE__);\
					   return -1;
int main(int argc, const char *argv[])
{
	FILE *fp;
	if((fp = fopen(argv[1], "r")) == NULL){
		errlog("fopen error");
	}

	fseek(fp, 0, SEEK_END);

	printf("The size of %s is %ld\n", argv[1], ftell(fp));
	fclose(fp);
	return 0;
}

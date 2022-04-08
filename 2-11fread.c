#include <stdio.h>
#include <string.h>
#include "source.h"
#define errlog(errmsg) perror(errmsg);\
	                   printf("--%s--%s--%d--\n",\ 
					   __FILE__, __FUNCTION__, __LINE__);\
					   return -1;
int main(int argc, const char *argv[])
{
	FILE *fp;
	struct data obj;
	if((fp = fopen("test.txt", "r")) == NULL){
		errlog("fopen error");
	}

	if(fread(&obj, sizeof(struct data), 1, fp) > 0){
		printf("a=%d b=%c buf=%s\n", obj.a, obj.b, obj.buf);
	}
	fclose(fp);
	return 0;
}

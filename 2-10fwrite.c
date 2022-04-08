#include <stdio.h>
#include <string.h>
#include "source.h"
#define errlog(errmsg) perror(errmsg);\
	                   printf("--%s--%s--%d--\n",\ 
					   __FILE__, __FUNCTION__, __LINE__);\
					   return -1;
struct data obj = {
	.a = 10,
	.b = 'q',
	.buf = "test",
};
int main(int argc, const char *argv[])
{
	FILE *fp;
	if((fp = fopen("test.txt", "w")) == NULL){
		errlog("fopen error");
	}

	if(fwrite(&obj, sizeof(struct data), 1, fp) < 0){
		errlog("fwrite error");
	}
	fclose(fp);
	return 0;
}

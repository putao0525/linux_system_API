#include <stdio.h>
#include <string.h>
#define N 32
#define errlog(errmsg) perror(errmsg);\
	                   printf("--%s--%s--%d--\n",\ 
					   __FILE__, __FUNCTION__, __LINE__);\
					   return -1;
int get_line(FILE *fp){
	char buf[N] = "";
	int count = 0;
	while(fgets(buf, N, fp) != NULL){
		if(buf[strlen(buf) - 1] == '\n')
			count++;
	}
	return count;
}

int main(int argc, const char *argv[])
{
	FILE *fp;
	int line = 0;
	if((fp = fopen(argv[1], "r")) == NULL){
		errlog("fopen error");
	}

	line = get_line(fp);

	printf("line = %d\n", line);
	fclose(fp);
	return 0;
}

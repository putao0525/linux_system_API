#include <stdio.h>
#define N 32
int main(int argc, const char *argv[])
{
	char buf[N] = "";
	while(fgets(buf, N, stdin) != NULL){
		fputs(buf, stdout);
	}
	return 0;
}

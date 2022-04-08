#include <stdio.h>
#include <unistd.h>

#define N 1024
#define errlog(errmsg) do{perror(errmsg);\
	                      printf("--%s--%s--%d--\n",\ 
					              __FILE__, __FUNCTION__, __LINE__);\
					      return -1;}while(0)
int main(int argc, const char *argv[])
{
	int i, fd[2];
	int count = 0;
	char buf[N] = "";
	for(i = 0; i < N; i++){
		buf[i] = 'q';
	}

	if(pipe(fd) < 0){
		errlog("pipe error");	
	}

	close(fd[0]);

	write(fd[1], buf, N);
	
	return 0;
}

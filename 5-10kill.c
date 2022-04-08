#include <stdio.h>
#include <signal.h>
#define errlog(errmsg) do{perror(errmsg);\
	                      printf("--%s--%s--%d--\n",\ 
					      __FILE__, __FUNCTION__, __LINE__);\
					      return -1;}while(0)
int main(int argc, const char *argv[])
{
	pid_t pid;

	pid = fork();

	if(pid < 0){
		errlog("fork error");
	}
	else if(pid == 0){
		printf("the child process\n");

		while(1)
			;
	}
	else{
		sleep(3);
		printf("the parent process\n");
		kill(pid, SIGKILL);
		raise(SIGKILL);
	}
	return 0;
}

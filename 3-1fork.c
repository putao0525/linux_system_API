#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, const char *argv[])
{
	pid_t pid;

	pid = fork();

	if(pid < 0){
		perror("fork error");
		return -1;
	}
	else if(pid == 0){
		/*child子进程的执行代码区*/
		printf("The child process\n");
	}
	else{
		/*parent父进程的执行代码区*/
		printf("The parent process\n");
	}
	return 0;
}

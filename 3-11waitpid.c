#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
	pid_t pid;

	pid = fork();

	if(pid < 0){
		perror("fork error");
		return -1;
	}
	else if(pid == 0){
		/*child*/
		printf("The child process, id = %d parent id = %d\n", 
				getpid(), getppid());
		sleep(5);
		exit(0);
	}
	else{
		/*parent*/
		int status;
		pid_t ret;

		while((ret = waitpid(pid, &status, WNOHANG)) == 0){
			sleep(1);
			printf("child has not been exited\n");
		}
		
		if(ret == pid){
			printf("child has been recycled\n");
		}
		printf("The parent process, id = %d\n", getpid());
		exit(0);
	}
	return 0;
}

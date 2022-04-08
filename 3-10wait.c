#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, const char *argv[])
{
	pid_t pid;
	int i = 3;

	pid = fork();

	if(pid < 0){
		perror("fork error");
		return -1;
	}
	else if(pid == 0){
		/*child*/
		printf("The child process, id = %d parent id = %d\n", 
				getpid(), getppid());
		while(i > 0){
			sleep(1);
			printf("child...\n");
			i--;
		}
	}
	else{
		/*parent*/
		int status;
		wait(&status);
		printf("The parent process, id = %d\n", getpid());
		while(1);
	}
	return 0;
}

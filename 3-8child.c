#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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
		while(1);
	}
	else{
		/*parent*/
		printf("The parent process, id = %d\n", getpid());
	}
	return 0;
}

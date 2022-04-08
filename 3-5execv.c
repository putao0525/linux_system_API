#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	char *array[4] = {"ls", "-l", "./file", NULL};
	if(execv("/bin/ls", array) < 0){
		perror("execl error");
		return -1;
	}	
	return 0;
}

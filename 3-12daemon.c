#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, const char *argv[])
{
	pid_t pid;
	int i, fd;
	char *buf = "This is a Daemon\n";

	pid = fork(); /*第一步*/
	if(pid < 0){
		printf("fork error\n");
		return -1;
	}
	else if(pid > 0){
		exit(0); /*父进程退出*/
	}
	else{
		setsid(); /*第二步*/
		chdir("/tmp"); /*第三步*/
		umask(0); /*第四步*/
		for(i = 0; i < getdtablesize(); i++){ /*第五步*/
			close(i);
		}

		/*守护进程创建完成，以下开始为进入守护进程的工作，仅作示例展示*/
		if((fd = open("daemon.log", O_CREAT|O_WRONLY|O_TRUNC, 0600)) < 0){
			perror("open error");
			return -1;
		}		
		while(1){
			write(fd, buf, strlen(buf));
			sleep(3);
		}
		close(fd);
	}
	return 0;
}


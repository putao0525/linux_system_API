#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define N 32
int main(int argc, const char *argv[])
{
	char buf[N] = "";
	int flags;

	/*修改文件描述符0,即标准输入*/
	flags = fcntl(0, F_GETFL);

	/*修改其标志位*/
	flags |= O_NONBLOCK;

	/*重新设置修改后的标志位*/
	fcntl(0, F_SETFL, flags);

	while(1){
		read(0, buf, N);
		buf[strlen(buf) - 1] = '\0';
		printf("read : %s\n", buf);

		sleep(1);
	}
	return 0;
}

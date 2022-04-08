#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	unsigned int ret;

	ret = alarm(8);
	printf("ret1 = %d\n", ret);

	sleep(3);
	ret = alarm(3);
	printf("ret2 = %d\n", ret);

	while(1)
		;
	return 0;
}

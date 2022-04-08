#include <stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;

	fp = fopen("test.txt", "w");

	fputc('a', fp);

	printf("fp buffer size:%ld\n", 
			fp->_IO_buf_end - fp->_IO_buf_base);
	getchar();
	printf("stdin buffer size:%ld\n", 
			stdin->_IO_buf_end - stdin->_IO_buf_base);
	printf("stdout buffer size:%ld\n", 
			stdout->_IO_buf_end - stdout->_IO_buf_base);
	printf("stderr buffer size:%ld\n", 
			stderr->_IO_buf_end - stderr->_IO_buf_base);
	return 0;
}

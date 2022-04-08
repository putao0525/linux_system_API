#include <stdio.h>

int main(int argc, const char *argv[])
{
	int ch;
	while((ch = fgetc(stdin)) != EOF){
		fputc(ch, stdout);
	}
	return 0;
}

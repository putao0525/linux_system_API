#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAXLEN 10 /*缓存区大小最大值*/
#define ALPHABET 1 /*表示使用英文字符*/
#define ALPHABET_START 'a' /*头一个字符，可以用'A'*/
#define COUNT_OF_ALPHABET 26 /*字母字符的个数*/
#define DIGIT 2 /*表示使用数字字符*/
#define DIGIT_START '0' /*头一个字符*/
#define COUNT_OF_DIGIT 10 /*数字字符的个数*/
#define SIGN_TYPE ALPHABET /*本示例选用英文字符*/
const char *fifo_file = "./myfifo";/*仿真FIFO文件名*/
char buf[MAXLEN];

int product(void){
	int fd;
	unsigned int sign_type, sign_start, sign_count, size;
	static unsigned int counter = 0;

	if((fd = open(fifo_file, O_RDWR|O_CREAT|O_APPEND, 0664)) < 0){
		perror("open error");
		return -1;
	}

	sign_type = SIGN_TYPE;

	switch(sign_type){
		case ALPHABET:
			sign_start = ALPHABET_START;
			sign_count = COUNT_OF_ALPHABET;
			break;
		case DIGIT:
			sign_start = DIGIT_START;
			sign_count = COUNT_OF_DIGIT;
			break;
		default:
			return -1;
	}
	sprintf(buf, "%c", (sign_start + counter));
	counter = (counter + 1) % sign_count;

	lock_set(fd, F_WRLCK);

	if((size = write(fd, buf, strlen(buf))) < 0){
		perror("Producer:write error");
		return -1;
	}

	lock_set(fd, F_UNLCK);

	close(fd);
	return 0;
}
int main(int argc, const char *argv[])
{
	int time_step = 1;/*生产周期*/
	int time_life = 10;/*需要生产的资源总数*/

	if(argc > 1){/*表示生产周期可通过命令行传参设置*/
		sscanf(argv[1], "%d", &time_step);
	}

	if(argc > 2){/*表示需要生产的资源总数可设置*/
		sscanf(argv[2], "%d", &time_life);
	}

	while(time_life--){
		if(product() < 0){
			break;
		}

		sleep(time_step);
	}
	return 0;
}

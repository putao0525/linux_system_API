#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define N 32
#define errlog(errmsg) do{perror(errmsg);\
						  printf("--%s--%s--%d--\n",\ 
								  __FILE__, __FUNCTION__, __LINE__);\
						  return -1;\
						 }while(0)
char buf[N] = "";
void *thread1_handler(void *arg){
	while(1){
		fgets(buf, N, stdin);
		buf[strlen(buf) - 1] = '\0';
	}
	pthread_exit("thread1...exit");
}
void *thread2_handler(void *arg){
	while(1){
		printf("buf:%s\n", buf);
		sleep(1);
	}
	pthread_exit("thread2...exit");
}

int main(int argc, const char *argv[])
{
	pthread_t thread1, thread2;
	if(pthread_create(&thread1, NULL, 
				thread1_handler, NULL) != 0){
		errlog("pthread_create1 error");
	}

	if(pthread_create(&thread2, NULL, 
				thread2_handler, NULL) != 0){
		errlog("pthread_create2 error");
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	return 0;
}

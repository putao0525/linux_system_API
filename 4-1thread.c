#include <stdio.h>
#include <pthread.h>

#define errlog(errmsg) do{perror(errmsg);\
						  printf("--%s--%s--%d--\n",\ 
								  __FILE__, __FUNCTION__, __LINE__);\
						  return -1;\
						 }while(0)
void *thread1_handler(void *arg){
	int count = *((int *)arg);

	while(count > 0){
		printf("thread1...\n");
		sleep(1);
		count--;
	}
	pthread_exit("thread1...exit");
}
void *thread2_handler(void *arg){
	int count = *((int *)arg);
	
	while(count > 0){
		printf("thread2...\n");
		sleep(1);
		count--;
	}
	pthread_exit("thread2...exit");
}

int main(int argc, const char *argv[])
{
	pthread_t thread1, thread2;
	int arg1 = 2;
	int arg2 = 5;
	void *retval;
	if(pthread_create(&thread1, NULL, 
				thread1_handler, (void *)&arg1) != 0){
		errlog("pthread_create1 error");
	}

	if(pthread_create(&thread2, NULL, 
				thread2_handler, (void *)&arg2) != 0){
		errlog("pthread_create2 error");
	}

	pthread_join(thread1, &retval);
	printf("%s\n", (char *)retval);

	pthread_join(thread2, &retval);
	printf("%s\n", (char *)retval);
	return 0;
}

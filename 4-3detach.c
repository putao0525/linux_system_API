#include <stdio.h>
#include <pthread.h>

#define errlog(errmsg) do{perror(errmsg);\
						  printf("--%s--%s--%d--\n",\ 
								  __FILE__, __FUNCTION__, __LINE__);\
						  return -1;\
						 }while(0)
void *thread_handler(void *arg){
	printf("thread...\n");
	return NULL;
}
int main(int argc, const char *argv[])
{
	pthread_t thread;
	pthread_attr_t attr;
	if(pthread_attr_init(&attr) != 0){
		errlog("pthread_attr_init error");
	}
	if(pthread_attr_setdetachstate(&attr, 
				PTHREAD_CREATE_DETACHED) != 0){
		errlog("pthread_attr_setdetachstate error");
	}
	if(pthread_create(&thread, &attr, 
				thread_handler, NULL) != 0){
		errlog("pthread_create1 error");
	}
	int temp = 0;
	sleep(1);
	if(pthread_join(thread, NULL) == 0){
		printf("pthread wait success\n");
		temp = 0;
	}
	else{
		printf("pthread wait failed\n");
		temp = 0;
	}
	return temp;
}

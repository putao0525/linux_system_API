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
pthread_cond_t cond;
pthread_mutex_t lock;
int count = 0;
void *thread1_handler(void *arg){
	while(1){
		printf("count = %d\n", ++count);
		sleep(1);
		pthread_mutex_lock(&lock);
		strcpy(buf, "hello");
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond);
	}
	pthread_exit(0);
}
void *thread2_handler(void *arg){
	while(1){
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&cond, &lock);
		printf("thread2 buf:%s\n", buf);
		sleep(1);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}
void *thread3_handler(void *arg){
	while(1){
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&cond, &lock);
		printf("thread3 buf:%s\n", buf);
		sleep(1);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}

int main(int argc, const char *argv[])
{
	pthread_t thread1, thread2, thread3;

	if(pthread_cond_init(&cond, NULL) != 0){
		errlog("pthread_cond_init error");
	}
	if(pthread_mutex_init(&lock, NULL) != 0){
		errlog("pthread_mutex_init error");
	}
	if(pthread_create(&thread1, NULL, 
				thread1_handler, NULL) != 0){
		errlog("pthread_create1 error");
	}

	if(pthread_create(&thread2, NULL, 
				thread2_handler, NULL) != 0){
		errlog("pthread_create2 error");
	}
	if(pthread_create(&thread3, NULL, 
				thread3_handler, NULL) != 0){
		errlog("pthread_create3 error");
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
	return 0;
}

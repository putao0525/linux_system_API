#include <stdio.h>
#include <pthread.h>

#define errlog(errmsg) do{perror(errmsg);\
						  printf("--%s--%s--%d--\n",\ 
								  __FILE__, __FUNCTION__, __LINE__);\
						  return -1;\
						 }while(0)
int value1 = 0;
int value2 = 0;
int count = 0;
pthread_mutex_t lock;

void cleanup_handler(void *arg){
	pthread_mutex_unlock(&lock);
}
void *thread1_handler(void *arg){
	while(1){
		pthread_mutex_lock(&lock);
		pthread_cleanup_push(cleanup_handler, NULL);
		value1 = count;
		value2 = count;
		count++;
		sleep(3);
		pthread_cleanup_pop(0);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}
void *thread2_handler(void *arg){
	while(1){
		sleep(1);
		pthread_mutex_lock(&lock);
		if(value1 == value2){
			sleep(1);
			printf("value1 = %d value2 = %d\n", value1, value2);
		}
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}


int main(int argc, const char *argv[])
{
	pthread_t thread1, thread2;
	void *retval;
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

	sleep(2);
	pthread_cancel(thread1);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_mutex_destroy(&lock);
	return 0;
}

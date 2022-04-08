#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

struct shmbuf{
	int a;
	char b;
};

int main(int argc, const char *argv[])
{
	key_t key;

	if((key = ftok(".", 'q')) < 0){
		perror("ftok error");
		return -1;
	}

	int shmid;
	struct shmbuf *shm;
	/*创建并打开共享内存*/
	if((shmid = shmget(key, 512, IPC_CREAT|IPC_EXCL|0664)) < 0){
		if(errno != EEXIST){
			perror("shmget error");
			return -1;
		}
		else{
			/*如果共享内存段已存在，则打开*/
			shmid = shmget(key, 512, 0664);
		}
	}
    /*用自定义的结构体指针接收函数的返回值*/
	if((shm = shmat(shmid, NULL, 0)) > 0){
		printf("shm:%p\n", shm);
	}

	/*向虚拟内存写入数据*/
	shm->a = 10;
	shm->b = 's';
	/*断开映射*/
	if(shmdt(shm) < 0){
		perror("shmdt error");
		return -1;
	}
	
	system("ipcs -m");
	return 0;
}

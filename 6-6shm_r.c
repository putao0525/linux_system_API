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
	if((shmid = shmget(key, 512, IPC_CREAT|IPC_EXCL|0664)) < 0){
		if(errno != EEXIST){
			perror("shmget error");
			return -1;
		}
		else{
			shmid = shmget(key, 512, 0664);
		}
	}

	if((shm = shmat(shmid, NULL, 0)) > 0){
		printf("shm:%p\n", shm);
	}

	/*从虚拟内存中读取数据*/
	printf("a = %d b = %c\n", shm->a, shm->b);
	
	if(shmdt(shm) < 0){
		perror("shmdt error");
		return -1;
	}
	/*删除共享内存*/	
	shmctl(shmid, IPC_RMID, NULL);

	system("ipcs -m");
	return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/sem.h>
#include <string.h>

#define N 128
struct shmbuf{
	char buf[N];
};
union semun{
	int val;
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

	int semid;
	union semun semun;

	struct sembuf sem;
	semid = semget(key, 2, IPC_CREAT|IPC_EXCL|0664);

	if(semid < 0){
		if(errno != EEXIST){
			perror("semget error");
			return -1;
		}
		else{
			semid = semget(key, 2, 0664);
		}
	}
	else{
		//初始化信号量的值
		semun.val = 0;
		semctl(semid, 0, SETVAL, semun);
		semun.val = 1;
		semctl(semid, 1, SETVAL, semun);
	}

	while(1){
		//申请写操作信号量
		sem.sem_num = 1;
		sem.sem_op = -1;
		sem.sem_flg = 0;
		semop(semid, &sem, 1);

		fgets(shm->buf, N, stdin);
		shm->buf[strlen(shm->buf) - 1] = '\0';


		//释放读操作信号量
		sem.sem_num = 0;
		sem.sem_op = 1;
		sem.sem_flg = 0;
		semop(semid, &sem, 1);

		if(strncmp(shm->buf, "quit", 4) == 0){
			goto ERR;
		}
	}
	return 0;
ERR:
	shmdt(shm);
}

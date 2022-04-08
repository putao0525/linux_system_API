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
	/*创建并打开共享内存*/
	if((shmid = shmget(key, 512, IPC_CREAT|IPC_EXCL|0664)) < 0){
		if(errno != EEXIST){
			perror("shmget error");
			return -1;
		}
		else{
			/*如果共享内存已存在，则打开*/
			shmid = shmget(key, 512, 0664);
		}
	}

	/*建立映射*/
	if((shm = shmat(shmid, NULL, 0)) > 0){
		printf("shm:%p\n", shm);
	}

	int semid;
	union semun semun;

	struct sembuf sem;
	/*创建并打开信号量集合*/
	semid = semget(key, 2, IPC_CREAT|IPC_EXCL|0664);

	if(semid < 0){
		if(errno != EEXIST){
			perror("semget error");
			return -1;
		}
		else{
			/*如果信号量集存在，则打开*/
			semid = semget(key, 2, 0664);
		}
	}
	else{
		/*初始化信号量的值
		 *将编号为0的信号量定义为读操作信号量
		 *读操作信号量初始化值为0
		 */
		semun.val = 0;
		semctl(semid, 0, SETVAL, semun);
		/*将编号为1的信号量定义为写操作信号量
		 *写操作信号量初始值为1
		 */
		semun.val = 1;
		semctl(semid, 1, SETVAL, semun);
	}

	while(1){
		//申请读操作信号量
		sem.sem_num = 0;
		sem.sem_op = -1;
		sem.sem_flg = 0;
		semop(semid, &sem, 1);

		if(strncmp(shm->buf, "quit", 4) == 0){
			goto ERR;
		}
		printf("buf:%s\n", shm->buf);

		//释放写操作信号量
		sem.sem_num = 1;
		sem.sem_op = 1;
		sem.sem_flg = 0;
		semop(semid, &sem, 1);
	}
	return 0;
ERR:
	shmdt(shm);
	shmctl(shmid, IPC_RMID, NULL);
	semctl(semid, 0, IPC_RMID, NULL);
	semctl(semid, 1, IPC_RMID, NULL);
}

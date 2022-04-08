#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#define errlog(errmsg) do{perror(errmsg);\
	                      printf("--%s--%s--%d--\n",\ 
					      __FILE__, __FUNCTION__, __LINE__);\
					      return -1;}while(0)
union semun{
	int val;
};
int main(int argc, const char *argv[])
{
	key_t key;

	if((key = ftok(".", 'q')) < 0){
		errlog("ftok error");
	}

	int semid;
	/*创建并打开信号量集，集合中有两个信号量*/
	if((semid = semget(key, 2, IPC_CREAT|IPC_EXCL|0665)) < 0){
		if(errno != EEXIST){
			errlog("semget error");
		}
		else{
		/*如果信号量集已存在，则打开即可*/
			semid = semget(key, 2, 0664);
		}
	}

	union semun semun;
	struct sembuf sem;
	/*初始化信号量的值,第一个信号量的编号为0,第二个信号量的编号为1*/
	semun.val = 1;
	semctl(semid, 0, SETVAL, semun);

	semun.val = 1;
	semctl(semid, 1, SETVAL, semun);

	/*对编号为0的信号量执行申请操作*/
	sem.sem_num = 0;
	sem.sem_op = -1;
	sem.sem_flg = 0;
	semop(semid, &sem, 1);

	/*对编号为1的信号量执行释放操作*/
	sem.sem_num = 1;
	sem.sem_op = 1;
	sem.sem_flg = 0;
	semop(semid, &sem, 1);

	/*获取信号量的值*/
	int retval;
	retval = semctl(semid, 0, GETVAL);
	printf("NO.0 retval = %d\n", retval);

	retval = semctl(semid, 1, GETVAL);
	printf("NO.1 retval = %d\n", retval);

	/*删除信号量*/
	semctl(semid, 0, IPC_RMID);
	semctl(semid, 1, IPC_RMID);

	return 0;
}

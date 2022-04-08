#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

#define N 128
#define SIZE sizeof(struct msgbuf) - sizeof(long)
struct msgbuf{
	long mtype;
	int a;
	char b;
	char buf[N];
};
int main(int argc, const char *argv[])
{
	key_t key;
/*创建key值*/
	if((key = ftok(".", 'a')) < 0){
		perror("ftok error");
		return -1;
	}
/*创建或打开消息队列*/
	int msqid;
	struct msgbuf msg;
	if((msqid = msgget(key, IPC_CREAT|IPC_EXCL|0664)) < 0){
		if(errno != EEXIST){
			perror("msgget error");
			return -1;
		}
		else{
			/*如果消息队列已存在，则打开消息队列*/
			msqid = msgget(key, 0664);
		}
	}
/*读取消息*/
	if(msgrcv(msqid, &msg, SIZE, 100, 0) < 0){
		perror("msgsnd error");
		return -1;
	}	

	printf("a = %d b = %c buf = %s\n", msg.a, msg.b, msg.buf);
/*删除消息队列*/
	msgctl(msqid, IPC_RMID, NULL);
/*调用shell命令产看系统中的消息队列*/
	system("ipcs -q");
	return 0;
}

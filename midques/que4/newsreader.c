#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<signal.h>
#include<stdlib.h>
#include<semaphore.h>

struct mesg_buf {
	long mesg_type;
	char mesg_text[100];
} mesg;

void ctrlC(int sig){
	 sem_unlink("mysem1");
	 printf("Unlinked!\n");
	 exit(1);
}

int main(int argc, char** argv)
{
	signal(SIGINT, ctrlC);
	char buf[100], fifo_path[100]; int val;
	sprintf(fifo_path, "%s%s", "/tmp/fifo", argv[1]);
	int key = ftok("/tmp/msgq", 68);
	int msg_id = msgget(key, 0666|IPC_CREAT);
	if(msg_id<0)
		printf("MsgQue failure\n");
	else
		printf("MsgQue success\n");
	mkfifo(fifo_path, 0666);
	int sfd = open(fifo_path, O_WRONLY);
	sem_t *sem = sem_open("mysem1", O_CREAT, 0644, 1);
	if(sem==SEM_FAILED)
		printf("Error: Semaphore failure!\n");
	else
		printf("Semaphore success\n");
	while(1){
		//sem_getvalue(sem, &val);
		//printf("Sem val: %d", val);
		sem_wait(sem);
		printf("Entered sem\n");
		msgrcv(msg_id, &mesg, sizeof(mesg), atoi(argv[1]), 0);
		printf("Received mesg\n");
		write(sfd, mesg.mesg_text, strlen(mesg.mesg_text));
		sem_post(sem);
		sleep(1);
	}
	return 0;

}
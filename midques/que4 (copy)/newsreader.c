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


int news_id;
int child, parent;
int sfd;

struct mesg_buf {
	long mesg_type;
	char mesg_text[100];
} mesg;


void readMsg(int sig){
		int key = ftok("/tmp/msgq", 68);
		int msg_id = msgget(key, 0666|IPC_CREAT);
		int key1 = ftok("/tmp/msgq1", 69);
		int msg_id1 = msgget(key1, 0666|IPC_CREAT);
		msgrcv(msg_id, &mesg, sizeof(mesg), news_id, 0);
		msgsnd(msg_id1, &mesg, sizeof(mesg), 0);
		//int k = write(sfd, mesg.mesg_text, strlen(mesg.mesg_text));
		//printf("Written %d bytes to FIFO\n", k);
		sleep(news_id);
		if(news_id==1)
			kill(child, SIGUSR1);
		else
			kill(parent, SIGUSR1);
}

int main(int argc, char** argv)
{
	news_id = atoi(argv[1]);
	if(news_id==1){
		if((child=fork())==0){
			char* args[] = {"news", "2", NULL};
			execvp("./news", args);
		}
	}
	else{
		parent = getppid();
	}
	signal(SIGUSR1, readMsg);
	char buf[100], fifo_path[100]; int val;
	/*sprintf(fifo_path, "%s%s", "/tmp/fifo", argv[1]);
	mkfifo(fifo_path, 0666);
	sfd = open(fifo_path, O_WRONLY);
	if(sfd>0)
		printf("Opened FIFO\n");
	else
		printf("FIFO Failure\n");*/
	printf("here\n");
	if(news_id==1)
		kill(child, SIGUSR1);

	while(1){}

	return 0;

}
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<string.h>
#include<semaphore.h>
#include<signal.h>
#include<stdlib.h>

int msg_id;

struct mesg_buf {
	long mesg_type;
	char mesg_text[100];
} mesg;

void ctrlC(int sig){
	 msgctl(msg_id, IPC_RMID, NULL); 
	 exit(1);
}

int main(){
	signal(SIGINT, ctrlC);
	int key = ftok("/tmp/msgq", 68), type=1;
	msg_id = msgget(key, 0666|IPC_CREAT);
	if(msg_id<0)
		printf("MsgQue failure\n");
	else
		printf("MsgQue success\n");
	int fd[3], maxfd=0;
	char buf[100];
	fd_set readfds;
	for (int i = 0; i < 3; i++)
	{
		char path[100];
		sprintf(path, "%s%d", "./r", i+1);
		fd[i] = fileno(popen(path, "r"));
		printf("Opened %d\n", fd[i]);
		if(fd[i]>maxfd)
			maxfd = fd[i];
	}

	while(1){
		FD_ZERO(&readfds);
		for(int i=0; i<3; i++){
			FD_SET(fd[i], &readfds);
		}

		select(maxfd+1, &readfds, NULL, NULL, NULL);

		for(int i=0; i<3; i++){
			if(FD_ISSET(fd[i], &readfds)){
				printf("Received from Reporter %d\n", i+1);
				read(fd[i], buf, 100);
				strcpy(mesg.mesg_text, buf); 
				mesg.mesg_type = type;
				if(type==1)
					type=2;
				else
					type=1;
				msgsnd(msg_id, &mesg, sizeof(mesg), 0);
			}
		}
		sleep(1);
	}
	

}
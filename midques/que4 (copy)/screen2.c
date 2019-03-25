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

struct mesg_buf {
	long mesg_type;
	char mesg_text[100];
} mesg;

int main(){
	int key = ftok("/tmp/msgq1", 69);
	int msg_id = msgget(key, 0666|IPC_CREAT);

	if(fork()==0){
		while(1){
			msgrcv(msg_id, &mesg, sizeof(mesg), 1, 0);
			printf("N1 sent: %s\n", mesg.mesg_text);
		}
	}
	else{
		while(1){
			msgrcv(msg_id, &mesg, sizeof(mesg), 2, 0);
			printf("N2 sent: %s\n", mesg.mesg_text);
		}
	}
	return 0;

}
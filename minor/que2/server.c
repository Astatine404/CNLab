#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct mesg_str {
	long mesg_type;
	char mesg_text[100];
} mesg;

int main(){
	key_t key = ftok("/tmp/msgq", 65);
	int msgid = msgget(key, 0666|IPC_CREAT);

	//accept clients
	int cl[2];
	for(int i=0; i<2; i++){
		//connect type 2
		msgrcv(msgid, &mesg, 100, 2, 0);
		cl[i] = atoi(mesg.mesg_text);
	}

	//start chat
	while(1){
		//received chat type 3
		msgrcv(msgid, &mesg, 100, 3, 0);
		printf("Received: %s\n", mesg.mesg_text);
		for(int i=0; i<2; i++){
			mesg.mesg_type = cl[i];
			msgsnd(msgid, &mesg, 100, 0);
			kill(cl[i], SIGUSR1);
		}
	}
	return 0;

}

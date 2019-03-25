#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgid;

struct mesg_str {
	long mesg_type;
	char mesg_text[100];
} mesg;

void func(int sig){
	msgrcv(msgid, &mesg, 100, getpid(), 0);
	printf("%s\n", mesg.mesg_text);
}

int main(){
	signal(SIGUSR1, func);
	char buf[100];
	key_t key = ftok("/tmp/msgq", 65);
	msgid = msgget(key, 0666|IPC_CREAT);

	//coonect to server
	sprintf(mesg.mesg_text, "%d", getpid());
	mesg.mesg_type = 2;
	msgsnd(msgid, &mesg, 100, 0);

	//start chat
	while(1){
		scanf("%s", mesg.mesg_text);
		mesg.mesg_type = 3;
		msgsnd(msgid, &mesg, 100, 0);
	}
	return 0;

}

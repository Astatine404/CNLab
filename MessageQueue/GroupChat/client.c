#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>


struct mesg_buffer {
	long mesg_type;
	char mesg_text[100];
} message;

int main(){
	key_t key;
	int msgid, grp;
	
	key = ftok("/tmp/server", 65);
	if(key<0)
		printf("Key Error!\n");
	
	msgid = msgget(key, 0666 | IPC_CREAT);
	printf("Enter grp no: "); scanf("%d", &grp);
	message.mesg_type = 999; //connect type
	//printf("here\n");
	//connect
	sprintf(message.mesg_text, "%d-%d", getpid(), grp); 
	if(msgsnd(msgid, &message, sizeof(message), 0)<0)
		printf("Connect Send Error\n");
	printf("%s, connected!\n", message.mesg_text);
	
	if(fork()==0){
		char msg[100];
		while(1){
			gets(msg);
			sprintf(message.mesg_text, "%d-%s", grp, msg);
			message.mesg_type = 1000; //client to server
	
			if(msgsnd(msgid, &message, sizeof(message), 0)<0)
				printf("Send Error\n");
		}
	}
	else{
		while(1){
			if(msgrcv(msgid, &message, sizeof(message), getpid(), 0)<0)
				printf("Receive Error\n");
			printf("%s\n", message.mesg_text);
		}
	}
	
	
	return 0;
}

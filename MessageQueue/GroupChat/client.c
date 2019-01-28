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
	printf("Enter grp no: "); scanf("%d", grp);
	message.mesg_type = 1; //connect type
	
	//connect
	sprintf(message.mesg_text, "%d-%d", getpid(), grp); 
	msgsnd(msgid, &message, sizeof(message), 0);
	printf("%s, connected!\n", message.mesg_text);
	
	if(fork()==0){
		while(1){
			gets(message.mesg_text);
			message.mesg_type = grp; //client to server
	
			msgsnd(msgid, &message, sizeof(message), 0);
		}
	}
	else{
		key = ftok("/tmp/server", 65);
		if(key<0)
			printf("Key Error!\n");
	
		msgid = msgget(key, 0666 | IPC_CREAT);
		while(1){
			if(msgrcv(msgid, &message, sizeof(message), getpid(), 0)<0)
				printf("Error\n");
			printf("%s\n", message.mesg_text);
		}
	}
	
	
	return 0;
}

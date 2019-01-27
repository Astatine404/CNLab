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
	int msgid;
	int client[10];
	int n = 0;
	
	key = ftok("/tmp/server", 65);
	if(key<0)
		printf("Key Error!\n");
	
	msgid = msgget(key, 0666 | IPC_CREAT);

	//connect phase
	while(n<2){
		msgrcv(msgid, &message, sizeof(message), 1, 0);
		printf("Connected to: %s\n", message.mesg_text);
		client[n++] = atoi(message.mesg_text);
	}
	
	
	while(1){
		msgrcv(msgid, &message, sizeof(message), 2, 0);
		printf("%s\n", message.mesg_text);
		for(int i=0; i<n; i++){
			message.mesg_type = client[i];
			if(msgsnd(msgid, &message, sizeof(message), 0)<0)
				printf("Error\n");
			//printf("Sent to %d\n", client[i]);
		}
		
	}
	

	msgctl(msgid, IPC_RMID, NULL);
	
	
	return 0;
}

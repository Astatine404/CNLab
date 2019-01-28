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
	int grp[100][100];
	int grpctr[100];
	for(int i=0; i<100; i++)
		grpctr[i] = 0;
	
	key = ftok("/tmp/server", 65);
	if(key<0)
		printf("Key Error!\n");
	
	msgid = msgget(key, 0666 | IPC_CREAT);

	//connect phase
	while(n<2){
		msgrcv(msgid, &message, sizeof(message), 1, 0);
		char currgrp[100], currclient[100];
		int i = 0, flag=0, delim=0;
		for(i=0; i<strlen(message.mesg_text); i++){
			if(message.mesg_text[i]=='-'){
				currclient[i] = '\0';
				flag = 1;
				delim = i;
			}
			else if(flag==0)
				currclient[i] = message.mesg_text[i];
			else if(flag==1){
				currgrp[i-delim-1] = message.mesg_text[i];
			}
		}
		currgrp[i-delim-1] = '\0';
		printf("Connected to: %s at group %s\n", currclient, currgrp);
		client[n++] = atoi(currclient);
		grp[atoi(currgrp)][grpctr[atoi(currgrp)]++] = atoi(currclient);
	}
	
	
	while(1){
		for(int i=0; i<2; i++){ //grp no
			if(msgrcv(msgid, &message, sizeof(message), i, IPC_NOWAIT)>0){
				printf("%s\n", message.mesg_text);
				for(int j=0; j<grpctr[i]; j++){ //clients per grp
					message.mesg_type = grp[i][j];
					if(msgsnd(msgid, &message, sizeof(message), 0)<0)
						printf("Error\n");
					//printf("Sent to %d\n", client[i]);
				}
			}
		}
		
	}
	

	msgctl(msgid, IPC_RMID, NULL);
	
	
	return 0;
}

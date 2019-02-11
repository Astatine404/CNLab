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
	int msgid, me, to;
	
	key = ftok("/tmp/server", 65);
	if(key<0)
		printf("Key Error!\n");
	
	msgid = msgget(key, 0666 | IPC_CREAT);
	
	printf("Who am I? ");
	scanf("%d", &me);
	
	printf("To? ");
	scanf("%d", &to);

	if(fork()==0){
		while(1){
			gets(message.mesg_text);
			message.mesg_type = to; //to p2
	
			if(msgsnd(msgid, &message, sizeof(message), 0)<0)
				printf("Send Error\n");
		}
	}
	else{
		while(1){
			if(msgrcv(msgid, &message, sizeof(message), me, 0)<0) //from p3
				printf("Receive Error\n");
			printf("%s\n", message.mesg_text);
		}
	}
	
	msgctl(msgid, IPC_RMID, NULL);
	
	
	return 0;
}

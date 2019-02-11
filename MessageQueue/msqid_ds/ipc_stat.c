#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<time.h>


struct mesg_buffer {
	long mesg_type;
	char mesg_text[100];
} message;

int main(){
	key_t key;
	int msgid;
	struct msqid_ds buf;
	
	key = ftok("/tmp/server", 65);
	if(key<0)
		printf("Key Error!\n");
	
	msgid = msgget(key, 0666 | IPC_CREAT);
	
	msgctl(msgid, IPC_STAT, &buf);
	
	printf("Last msgsnd: %s", ctime(&buf.msg_stime));
	printf("Last msgrcv: %s", ctime(&buf.msg_rtime));
	printf("Last change: %s", ctime(&buf.msg_ctime));
	printf("Number of messages in queue: %d\n", buf.msg_qnum);
	printf("Max bytes: %d\n", buf.msg_qbytes);
	printf("PID of last msgsnd: %d\n", buf.msg_lspid);
	printf("PID of last msgrcv: %d\n", buf.msg_lrpid);
	
	return 0;
	
}
	

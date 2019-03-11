#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/select.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>
#define PORT 8080

int ctr2=0;

void* func(void* arg){
	int* nsfd = (int*)arg;
	char buf[100], sbuf[100];
	recv(*nsfd, buf, sizeof(buf), 0);
	sprintf(sbuf, "%d\n", strlen(buf));
	send(*nsfd, sbuf, strlen(sbuf), 0);
	ctr2--;
}

int main(){
	int sfd1, sfd2, addrlen, max, nsfd;
	fd_set readfds;
	char buf[100];
	struct sockaddr_in address;

	sfd1 = socket(AF_INET, SOCK_STREAM, 0);
	sfd2 = socket(AF_INET, SOCK_STREAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT);

	bind(sfd1, &address, sizeof(address));
	address.sin_port = htons(PORT+1);
	bind(sfd2, &address, sizeof(address));
	listen(sfd1, 3);
	listen(sfd2, 3);

	int key = ftok("/tmp/shm", 65);
	int id = shmget(key, 1024, 0666|IPC_CREAT);
	int* ctr = (int*) shmat(id, 0, 0);
	*ctr = 0;

	if(sfd1>sfd2)
		max = sfd1;
	else
		max = sfd2;

	while(1){
		FD_ZERO(&readfds);
		FD_SET(sfd1, &readfds);
		FD_SET(sfd2, &readfds);

		select(max+1, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(sfd1, &readfds)){
			if(*ctr>=25)
				goto skip;
			nsfd = accept(sfd1, &address, &addrlen);
			printf("Client connect at 1\n");
			*ctr++;
			if(fork()==0){
				char old_in[10], old_out[10];
				sprintf(old_in, "%d", dup(0));
				sprintf(old_out, "%d", dup(1));
				dup2(nsfd, 0);
				dup2(nsfd, 1);
				char* args[] = {"s1", old_in, old_out, NULL};
				execvp("./s1", args);
			}
		}
		skip:

		if(FD_ISSET(sfd2, &readfds)){
			if(ctr2>=15)
				continue;
			nsfd = accept(sfd2, &address, &addrlen);
			ctr2++;
			printf("Client connect at 2\n");
			pthread_t Thread;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&Thread, &attr, func, (void*)&nsfd);

		}
	}

	return 0;
}
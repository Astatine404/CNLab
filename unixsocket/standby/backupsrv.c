#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

char* socket_path = "socket";

int usfd;

static int recv_fd(int sock){
	struct msghdr msg;
	struct iovec iov[1];
	struct cmsghdr *cmsg = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];

	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_controllen = CMSG_SPACE(sizeof(int));
	msg.msg_control = ctrl_buf;

	recvmsg(sock, &msg, 0);
	cmsg = CMSG_FIRSTHDR(&msg);

	return *((int *) CMSG_DATA(cmsg));
}

void* servicefunc(void* par){
	int* nsfd = (int*) par;
	int stat;
	char buf[100];

	while(1){
		stat = recv(*nsfd, buf, sizeof(buf), 0);
		if(stat<=0){
			printf("Thread killed\n");
			pthread_exit(0);
		}
		printf("strlen from backup: %d\n", (int)strlen(buf));
	}
}

int main(){
	struct sockaddr_un uaddr;

	usfd = socket(AF_UNIX, SOCK_STREAM, 0);

	uaddr.sun_family = AF_UNIX;
	strncpy(uaddr.sun_path, socket_path, sizeof(uaddr.sun_path)-1);

	bind(usfd, (struct sockaddr *)&uaddr, sizeof(uaddr));
	listen(usfd, 5);
	int mainsrv = accept(usfd, NULL, NULL);
	if(mainsrv<0){
		printf("Connection to main server failed\n");
		exit(0);
	}
	printf("Connection to main server active\n");

	while(1){
		char usfdstr[20];
		int nsfd = recv_fd(mainsrv);
		if(nsfd<=0)
			continue;
		printf("Client connected %d\n", nsfd);

		pthread_t Thread;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&Thread, &attr, servicefunc, &nsfd);
	}

	return 0;
}
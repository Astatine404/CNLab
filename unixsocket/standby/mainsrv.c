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
#include <sys/select.h>

char* socket_path = "socket";

int usfd;

void ctrlc(int sig){
	close(usfd);
	unlink(socket_path);
	exit(0);
}

int send_fd(int sock, int fd){
	struct msghdr msg;
	struct iovec iov[1];
	struct cmsghdr *cmsg = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];

	data[0] = ' ';
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_controllen = CMSG_SPACE(sizeof(int));
	msg.msg_control = ctrl_buf;

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(int));

	*((int *) CMSG_DATA(cmsg)) = fd;

	return sendmsg(sock, &msg, 0);
}

void* servicefunc(void* par){
	int* nsfd = (int*) par;
	int bck = *nsfd;
	char buf[100];
	int stat=1;

	while(1){
		stat = recv(bck, buf, sizeof(buf), 0);
		if(stat<=0){
			printf("Thread killed\n");
			pthread_exit(0);
		}
		printf("strlen from main: %d\n", (int)strlen(buf));
	}
}

int main(){
	int sfd, addrlen, maint=0;
	int fds[100], fdn=0;
	char buf[100];
	struct sockaddr_in iaddr, caddr;
	struct sockaddr_un uaddr;
	signal(SIGINT, ctrlc);

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	usfd = socket(AF_UNIX, SOCK_STREAM, 0);

	iaddr.sin_family = AF_INET;
	iaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	iaddr.sin_port = htons(8080);

	uaddr.sun_family = AF_UNIX;
	strncpy(uaddr.sun_path, socket_path, sizeof(uaddr.sun_path)-1);
	
	bind(sfd, (struct sockaddr *)&iaddr, sizeof(iaddr));
	listen(sfd, 5);

	connect(usfd, (struct sockaddr *)&uaddr, sizeof(uaddr));
	printf("Connection to backup server active\n");

	fd_set readfds;

	while(1){
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);
		FD_SET(sfd, &readfds);

		select(sfd+1, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(sfd, &readfds)){
			int nsfd = accept(sfd, (struct sockaddr *)&caddr, &addrlen);
			printf("Client connected\n");
			if(maint==0){
				pthread_t Thread;
				pthread_attr_t attr;
				pthread_attr_init(&attr);
				pthread_create(&Thread, &attr, servicefunc, &nsfd);
			}
			else{
				char fdstr[20];
				fds[fdn++] = nsfd;
				printf("Forwarding to Backup Server\n");
				send_fd(usfd, nsfd);
			}
		}

		if(FD_ISSET(0, &readfds)){
			scanf("%s", buf);
			if(!strcmp(buf, "maint")){
				printf("Started maintenance mode\n");
				maint = 1;
			}
			else if(!strcmp(buf, "stop")){
				char* sigstr = "switch";
				for(int i=0; i<fdn; i++){
					send(fds[i], sigstr, strlen(sigstr), 0);
				}
				fdn = 0;
				printf("Stopped maintenance mode\n");
				maint = 0;
			}
		}
	}

	return 0;
}
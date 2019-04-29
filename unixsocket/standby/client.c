#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <sys/select.h>

int main()
{
	struct sockaddr_in iaddr;
	char buf[100];

	int sfd = socket(AF_INET, SOCK_STREAM, 0);

	iaddr.sin_family = AF_INET;
	iaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	iaddr.sin_port = htons(8081);

	connect(sfd, (struct sockaddr *)&iaddr, sizeof(iaddr));
	printf("Connected to server\n");

	fd_set readfds;

	while(1){
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);
		FD_SET(sfd, &readfds);

		select(sfd+1, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(0, &readfds)){
			scanf("%s", buf);
			send(sfd, &buf, strlen(buf), 0);
		}

		if(FD_ISSET(sfd, &readfds)){
			recv(sfd, &buf, sizeof(buf), 0);
			if(!strcmp(buf, "switch")){
				close(sfd);
				int sfd = socket(AF_INET, SOCK_STREAM, 0);
				connect(sfd, (struct sockaddr *)&iaddr, sizeof(iaddr));
				printf("Reconnected to main server\n");
			}
		}
	}

	return 0;
}
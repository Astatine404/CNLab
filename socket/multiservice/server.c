#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<string.h>
#define PORT 8080

int main(){
	int sfd[4], nsfd, valread, addrlen, maxfd=0;
	fd_set sockfds;
	char buf[100];
	struct sockaddr_in address;
	for(int i=0; i<4; i++)
		sfd[i] = socket(AF_INET, SOCK_STREAM, 0);
	

	strcpy(buf, "Accepted\n");
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	for(int i=0; i<4; i++){
		address.sin_port = htons(PORT+i);
		bind(sfd[i], &address, sizeof(address));
		listen(sfd[i], 3);
	}

	maxfd = 0;
	for(int i=0; i<4; i++){
		if(sfd[i]>maxfd)
			maxfd = sfd[i];
	}
	
	while(1){
		FD_ZERO(&sockfds);
		for(int i=0; i<4; i++){
			FD_SET(sfd[i], &sockfds);
		}

		select(maxfd+1, &sockfds, NULL, NULL, NULL);
		for(int i=0; i<4; i++){
			if(FD_ISSET(sfd[i], &sockfds)){
				printf("Request for service %d received\n", i+1);
				nsfd = accept(sfd[i], &address, &addrlen);
				printf("Request for service %d from %d accepted\n", i+1, address.sin_port);
				send(nsfd, &buf, strlen(buf), 0);
			}
		}
	}
	
	return 0;
}

#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#define PORT 8080

int main(){
	int sfd, nsfd, valread, addrlen;
	char buf[100];
	struct sockaddr_in address;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sfd==0)
		printf("Socket fail\n");
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT);
	
	bind(sfd, &address, sizeof(address));
	listen(sfd, 3);
	
	while(1){
		nsfd = accept(sfd, &address, &addrlen);
		//in fork
		while(1){
			valread = read(nsfd, buf, 100);
			printf("%s\nReply: ", buf);
			scanf("%s", &buf);
			send(nsfd, buf, strlen(buf), 0);
		}
	}
	
	return 0;
}

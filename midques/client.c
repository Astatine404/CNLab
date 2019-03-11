#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#define PORT 8080

int main(int argc, char** argv){
	int sfd;
	char buf[100] = "HelloS1\n", sbuf[100];
	struct sockaddr_in address;

	sfd = socket(AF_INET, SOCK_STREAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT+atoi(argv[1]));

	connect(sfd, &address, sizeof(address));
	printf("Connected!\n");
	send(sfd, buf, strlen(buf), 0);
	printf("Sent!\n");

	recv(sfd, sbuf, sizeof(sbuf), 0);
	printf("Server says:%s\n", sbuf);
	//fflush(stdout);

	return 0;

}
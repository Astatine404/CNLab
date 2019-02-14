#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#define PORT 8080

int main(){
	struct sockaddr_in address, serv_addr;
	char buf[100];
	int sfd;
	
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sfd==0)
		printf("Socket fail\n");
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(PORT);
	
	connect(sfd, &serv_addr, sizeof(serv_addr));
	
	while(1){
		printf("Input: ");
		scanf("%s", buf);
		send(sfd, buf, strlen(buf), 0);
	}
	return 0;
}

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
	int sfd, serv_id;
	
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sfd==0)
		printf("Socket fail\n");
	
	printf("Service name[1/2/3/4]");
	scanf("%d", &serv_id);
	serv_id--;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(PORT+serv_id);
	
	connect(sfd, &serv_addr, sizeof(serv_addr));
	printf("Client Connected!\n");
	recv(sfd, &buf, 100, 0);
	printf("%s\n", buf);
	return 0;
}

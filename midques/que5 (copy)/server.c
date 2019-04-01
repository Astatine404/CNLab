#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8080

int main(){
	int sfd, nsfd, addrlen, s[3];
	struct sockaddr_in addr, c_addr;

	sfd = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(PORT);
	bind(sfd, &addr, sizeof(addr));
	listen(sfd, 3);

	while(1){

	}

}

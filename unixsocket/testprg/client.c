#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* socket_path = "socket";

int main(){
	struct sockaddr_un addr;
	char buf[100];

	int fd, rc;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);

	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
	connect(fd, &addr, sizeof(addr));
	while(1){
		scanf("%s", buf);
		send(fd, buf, strlen(buf), 0);
	}

	return 0;
}
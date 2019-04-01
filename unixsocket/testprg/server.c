#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

char* socket_path = "socket";
int fd;

void ctrlc(int sig){
	close(fd);
	unlink(socket_path);
	exit(0);
}

int main(){
	struct sockaddr_un addr;
	char buf[100];
	signal(SIGINT, ctrlc);

	int cfd;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);

	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
	bind(fd, &addr, sizeof(addr));
	listen(fd, 5);
	cfd = accept(fd, NULL, NULL);
	while(1){
		recv(cfd, buf, sizeof(buf), 0);
		printf("%s\n", buf);
	}

	return 0;
}
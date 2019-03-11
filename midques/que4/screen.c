#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<string.h>

int main()
{
	char buf[100];
	int maxfd;
	fd_set readfds;
	mkfifo("/tmp/fifo1", 0666);
	mkfifo("/tmp/fifo2", 0666);
	int sfd1 = open("/tmp/fifo1", O_RDONLY);
	int sfd2 = open("/tmp/fifo2", O_RDONLY);
	printf("All FIFO open to read\n");
	if(sfd1>sfd2)
		maxfd = sfd1;
	else
		maxfd = sfd2;

	while(1){
		FD_ZERO(&readfds);
		FD_SET(sfd1, &readfds);
		FD_SET(sfd2, &readfds);

		select(maxfd+1, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(sfd1, &readfds)){
			read(sfd1, buf, 100);
			printf("N1 says: %s\n", buf);
		}

		if(FD_ISSET(sfd2, &readfds)){
			read(sfd2, buf, 100);
			printf("N2 says: %s\n", buf);
		}
		sleep(1);
	}
	return 0;
}
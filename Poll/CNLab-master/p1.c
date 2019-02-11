#include<stdio.h>
#include<unistd.h>
#include<poll.h>

int main(){
	struct pollfd pfds[5];
	char buf[100];
	FILE *fd2 = popen("./p2", "r"), 
	*fdw = popen("./p6", "w"),
	*fd3 = popen("./p3", "r"),
	*fd4 = popen("./p4", "r"),
	*fd5 = popen("./p5", "r");
	if(!fd2||!fdw)
		printf("popen failed!");
	sleep(2);
	
	printf("POLLIN: %d, POLLOUT: %d, POLLERR: %d, POLLNVAL: %d, POLLHUP: %d\n", POLLIN, POLLOUT, POLLERR, POLLNVAL, POLLHUP);
	
	for(int i=0; i<4; i++){
		pfds[0].fd = fileno(fd2);
		pfds[0].events = POLLIN;

		pfds[1].fd = fileno(fd3);
		pfds[1].events = POLLIN;
	
		pfds[2].fd = fileno(fd4);
		pfds[2].events = POLLIN;
	
		pfds[3].fd = fileno(fd5);
		pfds[3].events = POLLIN;
	
		pfds[4].fd = fileno(fdw);
		pfds[4].events = POLLOUT;
		
		int i = poll(pfds, 5, 10000);
		printf("i: %d\n", i);
		if(pfds[0].revents & POLLIN)
		{
			printf("detected 0\n");
			fgets(buf, 100, fd2);
			if(pfds[4].revents & POLLOUT){
				fputs(buf, fdw);
				fputc('\n', fdw);
			}
	
		}
		else if(pfds[1].revents & POLLIN)
		{
			printf("detected 1\n");
			fgets(buf, 100, fd3);
			if(pfds[4].revents & POLLOUT){
				fputs(buf, fdw);
				fputc('\n', fdw);
			}
	
		}
		else if(pfds[2].revents & POLLIN)
		{
			printf("detected 2\n");
			fgets(buf, 100, fd4);
			if(pfds[4].revents & POLLOUT){
				fputs(buf, fdw);
				fputc('\n', fdw);
			}
	
		}
		else if(pfds[3].revents & POLLIN)
		{
			printf("detected 3\n");
			fgets(buf, 100, fd5);
			if(pfds[4].revents & POLLOUT){
				fputs(buf, fdw);
				fputc('\n', fdw);
			}
	
		}
		
		for(int i=0; i<4; i++){
			printf("Poll no %d says %d\n", i, pfds[i].revents);
			if(pfds[i].revents & POLLERR)
				printf("POLLERR%d\n", i);
			if(pfds[i].revents & POLLNVAL)
				printf("POLLNVAL%d\n", i);
			if(pfds[i].revents & POLLHUP)
				printf("POLLHUP%d\n", i);
			if(pfds[i].revents & POLLPRI)
				printf("POLLPRI%d\n", i);
		}
	}
	
	//printf("P1 received from %s\n", buf);
	
	pclose(fd2);
	pclose(fd3);
	pclose(fd4);
	pclose(fd5); 
	pclose(fdw);
	sleep(6);
	
	return 0;

}

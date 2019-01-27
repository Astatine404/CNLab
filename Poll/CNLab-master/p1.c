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
			printf("here\n");
			fgets(buf, 100, fd2);
			if(pfds[4].revents & POLLOUT){
				fputs(buf, fdw);
				fputc('\n', fdw);
			}
	
		}
		else if(pfds[1].revents & POLLIN)
		{
			fgets(buf, 100, fd3);
			if(pfds[4].revents & POLLOUT){
				fputs(buf, fdw);
				fputc('\n', fdw);
			}
	
		}
		else if(pfds[2].revents & POLLIN)
		{
			fgets(buf, 100, fd4);
			if(pfds[4].revents & POLLOUT){
				fputs(buf, fdw);
				fputc('\n', fdw);
			}
	
		}
		else if(pfds[3].revents & POLLIN)
		{
			fgets(buf, 100, fd5);
			if(pfds[4].revents & POLLOUT){
				fputs(buf, fdw);
				fputc('\n', fdw);
			}
	
		}
		
		if(pfds[0].revents & POLLERR)
			printf("POLLERR");
		if(pfds[0].revents & POLLNVAL)
			printf("POLLNVAL");
		if(pfds[0].revents & POLLHUP)
			printf("POLLHUP");
		if(pfds[0].revents & POLLPRI)
			printf("POLLPRI");
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

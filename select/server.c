#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/select.h>
#include<string.h>

int main(){
	fd_set readfds, writefds;
	char msg[100];
	int activity, max;
	
	FILE *fdw = popen("./reader", "w"),
	*fdr1 = popen("./writer1", "r"),
	*fdr2 = popen("./writer2", "r"),
	*fdr3 = popen("./writer3", "r");
	
	int fdwn = fileno(fdw),
	fdr1n = fileno(fdr1),
	fdr2n = fileno(fdr2),
	fdr3n = fileno(fdr3);
	
	if(fdr1n>fdr2n)
		max = fdr1n;
	else
		max = fdr2n;
		
	if(fdr3n>max)
	max = fdr3n;
	
	for(int i=0; i<3; i++){
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(fdwn, &writefds);
		FD_SET(fdr1n, &readfds);
		FD_SET(fdr2n, &readfds);
		FD_SET(fdr3n, &readfds);
		printf("i: %d\n", i);
		
		activity = select(max+1, &readfds, NULL, NULL, NULL);
		
		if(activity<0){
			printf("select error\n");
			return 1;
		}
		else
			printf("activity: %d\n", activity);
		
		if(FD_ISSET(fdr1n, &readfds)){
			printf("read detected on 1\n");
			read(fdr1n, msg, sizeof(msg));
			printf("read from 1\n");
			int len = strlen(msg);
			//msg[len] = '\n';
			if(len>2)
			printf("%s\n", msg);
					
		}
		
		else if(FD_ISSET(fdr2n, &readfds)){
			printf("read detected on 2\n");
			read(fdr2n, msg, sizeof(msg));
			int len = strlen(msg);
			//msg[len] = '\n';
			if(len>2)
			printf("%s\n", msg);
		}
		
		else if(FD_ISSET(fdr3n, &readfds)){
			printf("read detected on 3\n");
			read(fdr3n, msg, sizeof(msg));
			int len = strlen(msg);
			//msg[len] = '\n';
			if(len>2)
			printf("%s\n", msg);
		}
		
	}
	pclose(fdw);
	pclose(fdr1);
	pclose(fdr2);
	pclose(fdr3);
	
	return 0;
	
	
}


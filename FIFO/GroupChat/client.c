#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>

int main(){
	char msg[100];
	char cname[100];
	char base[100];
	char grp[100];
	
	printf("Enter client name:");
	gets(cname);
	strcpy(base, "/tmp/");
	strcat(base, cname); 

	printf("Enter grp name:");
	gets(msg);
	strcpy(grp, "/tmp/");
	strcat(grp, msg);

	if(mkfifo(base, 0666)<0){
		printf("Error: %s\n", strerror(errno));
		
	}

	printf("Opened FIFO at %s\n", base);
	sprintf(cname, "%s-%s", base, msg);
	//connect to server
	int bytes=-1;
	
	while(bytes<=0){
		int serv = open("/tmp/server", O_WRONLY);
		if(serv<0)
			printf("File won't open!\n");
		else if((bytes=write(serv, cname, sizeof(cname)))<0){
			printf("Connect Error: %s\n", strerror(errno));
		}
		else{
			
			printf("%d bytes written to server\n", bytes);
			
		}
		close(serv);
	}
	
	//connected
	if(fork() > 0){
		while(1){
			int rfd = open(base, O_RDONLY);
			if(!rfd){
				printf("File won't open!\n");
			}
			else if(read(rfd, msg, sizeof(msg))>0){
				printf("%s\n", msg);
			}
			close(rfd);
			sleep(1);
		}
	}
	else{
		char snd[100];
		while(1){
			gets(snd);
			int serv = open(grp, O_WRONLY);
			if(serv>=0){
				write(serv, snd, sizeof(snd));
				printf("Written to %s...\n", grp);
			}
			else
				printf("Cannot write to %s...\n", grp);
			close(serv);
			sleep(1);
		}
	}
	return 0;
}

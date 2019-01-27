#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>

int main(){
	char msg[100];
	char cname[100];
	char base[100];
	
	gets(cname);
	strcpy(base, "/tmp/");
	strcat(base, cname);

	if(mkfifo(base, 0666)<0){
		printf("Error: %s\n", strerror(errno));
		
	}

	printf("Opened FIFO at %s\n", base);
	
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
			int serv = open("/tmp/server", O_WRONLY);
			write(serv, snd, sizeof(snd));
			close(serv);
			sleep(1);
		}
	}
	return 0;
}

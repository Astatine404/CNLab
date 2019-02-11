#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>

int main(){
	char msg[100];
	char base[100];
	char client[5][100];
	int n = 0;
	char* accept ="Accepted";
	int bytes;

	if(mkfifo("/tmp/server", 0666)<0){
		printf("Error: %s\n", strerror(errno));
	}

	
	int rfd = open("/tmp/server", O_RDONLY);
	while(n<3){
		if(read(rfd, msg, 100)>0){
			strcpy(base, "/tmp/");
			strcat(base, msg);
			printf("%s\n", msg);
			printf("Connected to: %s\n", base);
			strcpy(client[n++], base);
			int wfd = open(base, O_WRONLY);
			if(write(wfd, accept, sizeof(accept))>0){
				printf("Server: Wrote to client\n");
			}
			else{
				printf("Server: Connect Error: %s\n", strerror(errno));
			}
			close(wfd);
			printf("Client %d done\n", n);
		}
	}
	close(rfd);
	
	while(1){
		int rfd = open("/tmp/server", O_RDONLY);
		if(rfd<0){
			printf("File won't open!\n");
		}
		else if(read(rfd, msg, sizeof(msg))>0){
			printf("%s\n", msg);
			for(int i=0; i<n; i++){
				int wfd = open(client[i], O_WRONLY);
				write(wfd, msg, sizeof(msg));
			}
		}
		
		close(rfd);
		sleep(1);
	}
	
	return 0;
}

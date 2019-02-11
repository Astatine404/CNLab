#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>

int main(){
	char msg[100];
	char base[100];
	char grpPath[5][100];
	char* grp[5][100];
	int grpctr[5];
	int n = 0;
	char* accept ="Accepted";
	int bytes;

	for(int i=0; i<5; i++){
		grpctr[i] = 0;
		sprintf(grpPath[i], "/tmp/g%d", i);
		mkfifo(grpPath[i], 0666);
	}
	
	if(mkfifo("/tmp/server", 0666)<0){
		printf("Error: %s\n", strerror(errno));
	}

	
	
	while(n<3){
		int rfd = open("/tmp/server", O_RDONLY);
		if(read(rfd, msg, 100)>0){
			//printf("%s\n", msg);
			int flag=0, i=0, delim=0; char currgrp[100];
			for(i=0; i<strlen(msg); i++){
				if(msg[i]=='-'){
					flag = 1;
					delim = i;
					base[i] = '\0';
				}
				else if(flag==0){
					base[i] = msg[i];
				}
				else if(flag==1){
					currgrp[i-delim-1] = msg[i];
				}
			}
			currgrp[i-delim-1] = '\0';
			printf("Connected to %s at %s\n", base, currgrp);
			int grpno = (int)currgrp[i-delim-2]-(int)'0';
			printf("%d\n", grpno);
			grp[grpno][grpctr[grpno]] = malloc(100*sizeof(char));
			strcpy(grp[grpno][grpctr[grpno]++], base);
			int wfd = open(base, O_WRONLY);
			if(write(wfd, accept, sizeof(accept))>0){
				printf("Server: Accepted client\n");
			}
			else{
				printf("Server: Connect Error: %s\n", strerror(errno));
			}
			close(wfd);
			printf("Client %d done\n", n++);
		}
		close(rfd);
	}
	
	for(int i=0; i<5; i++){
		if(fork()==0){
			//printf("Reading for %s\n", grpPath[i]);
			while(1){
				//printf("Checking %s...\n", grpPath[i]);
				int rfd = open(grpPath[i], O_RDONLY);
				if(rfd<0){
					printf("File won't open!\n");
				}
				else if(read(rfd, msg, sizeof(msg))>0){
					printf("%s\n", msg);
					printf("%d\n", grpctr[i]);
					for(int j=0; j<grpctr[i]; j++){
						//printf("Writing to %s...\n", grp[i][j]);
						int wfd = open(grp[i][j], O_WRONLY);
						if(write(wfd, msg, sizeof(msg))>0)
							printf("Written to %s\n", grp[i][j]);
						else
							printf("Write failed to %s\n", grp[i][j]);
					}
				}
	
				close(rfd);
				sleep(1);
			}
		}
	}
	
	
	wait(NULL);
	return 0;
}

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>

int main(){
	char msg[100];
	int bytes;

	if(mkfifo("/tmp/server", 0666)<0){
		printf("Error: %s\n", strerror(errno));
	}
	
	
	int rfd = open("/tmp/server", O_RDONLY);
	
	while(n<3){
		if(read(rfd, msg, 100)>0){
			char source[100], dest[100]; int i;
			strcpy(source, "./tmp/d");
			source[6] = msg[0]; 
			source[7] = '\0';
			
			for(i=1; i<strlen(msg); i++)
				dest[i-1] = msg[i];
			dest[i] = '\0';
			
			
			if(fork()==0){
				int sfd = popen(source, "r");
				int dfd = open(dest, O_RDONLY);
				while(1){
					if(read(rfd, msg, sizeof(msg))>0){
						write(dfd, msg, strlen(msg));
					}
				}
			}
		}
	}
	
	return 0;
	
}

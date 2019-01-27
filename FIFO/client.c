#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
	char msg[100];
	
	while(1){
		int rfd = open("/tmp/server", O_RDONLY);
	
		read(rfd, msg, 100);
		close(rfd);
		printf("%s", msg);
	}
	
	return 0;
}

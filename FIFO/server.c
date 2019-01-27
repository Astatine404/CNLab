#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>

int main(){
	char* msg = "Hello";

	if(mkfifo("/tmp/server", 0666)<0){
		printf("Error: %s\n", strerror(errno));
	}
	
	while(1){
		int wfd = open("/tmp/server", O_WRONLY);
		write(wfd, msg, 100);
		close(wfd);
	}
	
	return 0;
}

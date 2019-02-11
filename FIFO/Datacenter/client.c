#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>

int main(){
	char msg[100];
	char client[100], tmp[100], dest[100];
	int bytes;
	
	strcpy(client, "/tmp/");
	printf("Client Name: ");
	scanf("%s", tmp);
	strcat(client, tmp);
	

	if(mkfifo(client, 0666)<0){
		printf("Error: %s\n", strerror(errno));
	}

	printf("Datacenter Number: (1,2,3) ");
	scanf("%s", tmp);
	
	strcpy(msg, tmp);
	strcat(msg, client);
	
	int wfd = open("/tmp/server", O_WRONLY);
	write(wfd, msg, sizeof(msg));
	
	return 0;
	
}

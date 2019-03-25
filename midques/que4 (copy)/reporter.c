#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
	char buf[100];
	while(1){
		sprintf(buf, "Mesg: %d", rand()%1000);
		write(1, buf, strlen(buf));
		sleep(2);
	}
	return 0;
}
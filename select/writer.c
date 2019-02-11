#include<stdio.h>

int main(){
	char msg[100];
	sleep(1);
	
	printf("I am %d\n", getpid());
	fflush(stdout);
	while(1);
	return 0;

}

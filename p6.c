#include<stdio.h>
#include<unistd.h>

int main(){
	char buf[100];
	for(int i=0; i<4; i++){
		printf("Waiting for scanner\n");
		gets(buf);
		printf("Received scanner\n");
		printf("P6 received from %s\n", buf);
	}
	sleep(5);
	return 0;
}

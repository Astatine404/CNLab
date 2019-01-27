#include<unistd.h>
#include<stdio.h>
#include<string.h>

int main(){
	int pp1[2], pp2[2];
;
	char* msg="Checking1";
	char buf[100];
	if(pipe(pp1)*pipe(pp2)<0){
		printf("Pipe Failed!");
	}
	int c = fork();
	if(c>0){
		close(pp1[0]);
		close(pp2[1]);
		write(pp1[1], msg, 100);
		printf("Written to child: %s\n", msg);
		sleep(1);
		read(pp2[0], buf, 100);
		printf("Received from child: %s\n", buf);
		wait(NULL);
	}
	else{
		close(pp1[1]);
		close(pp2[0]);
		//msg[8] = '2';
		write(pp2[1], msg, 100);
		printf("Written to parent: %s\n", msg);
		sleep(1);
		read(pp1[0], buf, 100);
		printf("Received from parent: %s\n", buf);
	}
	
	return 0;


}

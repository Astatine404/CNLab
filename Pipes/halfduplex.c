#include<unistd.h>
#include<stdio.h>

int main(){
	int pp[2];
	char* msg="Checking";
	char buf[100];
	if(pipe(pp)<0){
		printf("Pipe Failed!");
	}
	int c = fork();
	if(c>0){
		close(pp[0]);
		write(pp[1], msg, 100);
		printf("Written: %s\n", msg);
		wait(NULL);
	}
	else{
		close(pp[1]);
		sleep(1);
		read(pp[0], buf, 100);
		printf("Read: %s\n", buf);
	}
	
	return 0;


}

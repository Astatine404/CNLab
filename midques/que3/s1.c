#include<stdio.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

int main(int argc, char** argv){
	char buf[100];
	read(0, buf, 100);
	write(atoi(argv[2]), buf, strlen(buf));
	read(atoi(argv[1]), buf, 100);
	write(1, buf, strlen(buf));

	int key = ftok("/tmp/shm", 65);
	int id = shmget(key, 1024, 0666|IPC_CREAT);
	int* ctr = (int*) shmat(id, 0, 0);
	*ctr--;

	return 0;
}
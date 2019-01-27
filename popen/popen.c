#include<unistd.h>
#include<stdio.h>

int main(){
	int fd = popen("./test", 'w');

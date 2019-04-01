#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<string.h>
#include<semaphore.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


int pp[2]
pipe(pp)
pp[0] --> read end
pp[1] --> write end

dup(fd) --> returns copy of fd
dup2(oldfd, newfd) --> copy oldfd to newfd

fd = popen("path", "r") --> read from stdout  "w" --> write to stdin

struct pollfd pfds[10]
pfds[i].events = POLLIN/POLLOUT
pfds[i].fd = ofd
poll(pfds, n, timeout)
if(pfds[i].revents & POLLIN){}

execl("./path", "path args", (char*)0)
signal(SIGUSR1, void* func)
kill(pid, signalid)

mkfifo(path, 0666)
open(path, O_RDONLY/O_WRONLY)

struct mesg_buf {
	long mesg_type;
	char mesg_text[100];
} mesg;

key = ftok(path, 65)
msgid = msgget(key, 0666|IPC_CREAT)            shmget(key, 0666|IPC_CREAT)
msgsnd(msgid, &mesg, sizeof(mesg), 0)		   void* str = shmat(shmid, 0, 0)
msgrcv(msgid, &mesg, sizeof(mesg), typeid, 0); shmdt(str);


fd_set readfs;
FD_ZERO(&readfds)
FD_SET(fd, &readfds)
select(max_fd+1, &readfds, NULL, NULL, NULL)
if(FD_ISSET(fd, &readfds))


//client
struct sockaddr_in address;
sfd = socket(AF_INET, SOCK_STREAM, 0)
address.sin_family = AF_INET
address.sin_addr.s_addr = inet_addr("127.0.0.1")
address.sin_port = htons(port);

connect(sfd, &address, sizeof(address))
send(sfd, buf, strlen(buf), 0);
recv(sfd, &buf, 100, 0)

//server
socket()
bind(sfd, &address, sizeof(address))
listen(sfd, 3);
nsfd = accept(sfd, &caddr, &caddrlen)

sem_t *sem = sem_open("mysem1", O_CREAT, 0644, 1);
sem_wait(sem);
sem_post(sem);

inet_ntoa(c_addr.sin_addr); 
ntohs(c_addr.sin_port);
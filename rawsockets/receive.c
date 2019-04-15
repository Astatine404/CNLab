#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>	
#include <arpa/inet.h>
#include <netinet/in.h>

struct pseudo_header
{
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t tcp_length;
};

unsigned short csum(unsigned short *ptr,int nbytes) 
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}

int main (void)
{
	
	int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
	
	if(s == -1)
	{
		perror("Failed to create socket");
		exit(1);
	}
	
	char datagram[4096] , source_ip[32] , *data , *pseudogram;
	
	//clear buffer
	memset (datagram, 0, 4096);
	
	struct iphdr *iph = (struct iphdr *) datagram;
	
	//TCP header
	struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
	struct sockaddr_in sin;
	struct pseudo_header psh;

	data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
	strcpy(data , "00000000000000"); //dummy data
	
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8081);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//IP_HDRINCL to tell the kernel that headers are included in the packet
	int one = 1;
	const int *val = &one;
	int addrlen;
	int totlen = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
	
	setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one));
	
	while (1)
	{
		if (recvfrom (s, datagram, totlen, 0, (struct sockaddr *) &sin, &addrlen) < 0)
		{
			perror("receive failed");
		}
		else
		{
			write (1, data, strlen(data));
			printf("\n");
		}
	}
	
	return 0;
}
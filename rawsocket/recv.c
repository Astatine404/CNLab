#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct pseudo_header{
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t tcp_length;
};

//Checksum calculation
unsigned short csum(unsigned short *ptr, int nbytes){
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;

	while(nbytes>1){
		sum += *ptr++;
		nbytes -= 2;
	}

	if(nbytes==1){
		oddbyte = 0;
		*((u_char*)&oddbyte) = *(u_char*)ptr;
		sum += oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer = (short) - sum;

	return (answer);
}

int main(){
	int sfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);

	if(sfd==-1){
		printf("Raw socket failed\n");
		exit(1);
	}

	char datagram[4096], *data, *pseudogram;
	memset(datagram, 0, 4096);

	struct iphdr *iph = (struct iphdr *) datagram;
	struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof(struct ip));
	struct sockaddr_in sin;
	struct pseudo_header psh;

	data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
	strcpy(data, "OLDtxt");

	sin.sin_family = AF_INET;
	sin.sin_port = htons(8080);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
	pseudogram = malloc(psize);

	memcpy(pseudogram, (char*)&psh, sizeof(struct pseudo_header));
	memcpy(pseudogram + sizeof(struct pseudo_header), tcph, sizeof(struct tcphdr) + strlen(data));

	tcph->check = csum((unsigned short *)pseudogram, psize);
	int one = 1;
	const int *val = &one;

	setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));
	int tl = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data);
	int addrlen;
	printf("Waiting...\n");

	while(1){
		int stat = recvfrom(sfd, datagram, tl, 0, (struct sockaddr *)&sin, &addrlen);
		if(stat<0)
			printf("Receive Failed\n");
		else{
			printf("Received: %s\n", data);
		}
	}

	return 0;
}
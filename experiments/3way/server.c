#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>	
#include <arpa/inet.h>
#include <netinet/in.h>

/*struct pseudo_header
{
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t tcp_length;
};*/

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
	int addrlen;
	
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
	struct sockaddr_in sin, sout;
	//struct pseudo_header psh;
	
	//Data part
	data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
	strcpy(data , "Hello\n");
	
	strcpy(source_ip , "127.0.0.3");
	sout.sin_family = AF_INET;
	sout.sin_port = htons(8081);
	sout.sin_addr.s_addr = inet_addr("127.0.0.2");


	sin.sin_family = AF_INET;
	sin.sin_port = htons(8082);
	sin.sin_addr.s_addr = inet_addr("127.0.0.3");

	bind(s, (struct sockaddr *)&sin, sizeof(sin));
	
	//Fill in the IP Header
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
	iph->id = htonl (54321);	//random packet id
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol = IPPROTO_TCP;
	iph->check = 0;
	iph->saddr = inet_addr ( source_ip );	//Spoofed source IP
	iph->daddr = sout.sin_addr.s_addr;
	
	//Ip checksum
	//iph->check = csum ((unsigned short *) datagram, iph->tot_len);
	
	//TCP Header
	tcph->source = htons (8082);
	tcph->dest = htons (8081);
	tcph->seq = 0;
	tcph->ack_seq = 0;
	tcph->doff = 5;	//tcp header size
	tcph->fin=0;
	tcph->syn=1;
	tcph->rst=0;
	tcph->psh=0;
	tcph->ack=0;
	tcph->urg=0;
	tcph->window = htons (5840);	/* maximum allowed window size */
	tcph->check = 0;
	tcph->urg_ptr = 0;
	
	//Now the TCP checksum
	/*psh.source_address = inet_addr( source_ip );
	psh.dest_address = sin.sin_addr.s_addr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data) );
	
	int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
	pseudogram = malloc(psize);
	
	memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
	memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr) + strlen(data));
	
	tcph->check = csum( (unsigned short*) pseudogram , psize);*/
	
	//IP_HDRINCL to tell the kernel that headers are included in the packet
	int one = 1;
	const int *val = &one;
	
	setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one));
	int totlen = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
	
	while (1)
	{
		//Send the packet
		if (sendto (s, datagram, iph->tot_len ,	0, (struct sockaddr *) &sout, sizeof (sout)) < 0)
		{
			perror("send failed");
		}
		//Data send successfully
		else
		{
			printf ("Sent SYN\n");
			break;
		}
	}

	while (1)
	{
		if (recvfrom (s, datagram, totlen , 0, (struct sockaddr *) &sin, &addrlen) < 0){
			perror("recv failed");
		}
		else if(tcph->ack==1&&tcph->syn==1){
				printf("Received SYN-ACK ");
				break;
		}
	}

	tcph->syn = 0;
	tcph->ack_seq = tcph->seq + 1;
	tcph->ack = 1;
	sin.sin_addr.s_addr = iph->saddr;
	printf("from %s:%d\n", inet_ntoa(sin.sin_addr), ntohs(tcph->source));
	tcph->source = htons(8082);
	tcph->dest = htons(8081);
	iph->saddr = inet_addr("127.0.0.3");
	iph->daddr = inet_addr("127.0.0.2");
	sleep(2);

	while (1)
	{
		//Send the packet
		if (sendto (s, datagram, totlen ,0, (struct sockaddr *) &sout, sizeof (sout)) < 0)
		{
			perror("send failed");
		}
		//Data send successfully
		else
		{
			printf ("Sent ACK\n");
			break;
		}
	}

	sleep(2);
	
	return 0;
}
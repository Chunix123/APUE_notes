#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define DEST_PORT 6001
#define SRC_PORT 6002
#define SRC_ADDRESS "192.168.9.182"
#define DEST_ADDRESS "192.168.9.182"

//tcp伪首部
struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
};

/*
  Generic checksum calculation function
*/
unsigned short csum(unsigned short *ptr, int nbytes)
{
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum = 0;
    while(nbytes > 1)
    {
        sum += *ptr++;
        nbytes -= 2;
    }
    
    if(nbytes==1) 
    {
        oddbyte=0;
        *((u_char*)&oddbyte) = *(u_char*)ptr;
        sum += oddbyte;
    }

    sum = (sum>>16) + (sum & 0xffff);
    sum = sum + (sum>>16);
    answer = (short)~sum;

    return(answer);
}

int main()
{
    //Create a raw socket
    int s = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    
    if(s == -1)
    {
        perror("Failed to create socket\n");
        exit(1);
    }
    
    //Datagram to represent the packet
    char datagram[4096], source_ip[32], *data, *pseudogram;
    
    memset(datagram, 0, sizeof(datagram));
    //IP Header
    struct iphdr *iph = (struct iphdr*)datagram;
    //TCP Header
    struct tcphdr *tcph = (struct tcphdr*)(datagram + sizeof(struct iphdr));
    struct sockaddr_in sin;
    struct pseudo_header psh;
    //Data part
    data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
    strcpy(data , "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    strcpy(source_ip, SRC_ADDRESS);//源ip地址
    sin.sin_family = AF_INET;
    sin.sin_port = htons(DEST_PORT);
    sin.sin_addr.s_addr = inet_addr(DEST_ADDRESS);

    //Fill in the IP Header
    iph->ihl = 5;// 20 byte
    iph->version = 4;//ipv4
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data);
    iph->id = htonl(54321);//ID
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;
    iph->saddr = inet_addr(source_ip);
    iph->daddr = sin.sin_addr.s_addr;

    //IP checksum
    iph->check = csum((unsigned short*)datagram, sizeof(struct iphdr));

    //TCP header
    tcph->source = htons(SRC_PORT);
    tcph->dest = htons(DEST_PORT);
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5; //tcp header size
    tcph->fin = 0;
    tcph->syn = 1;
    tcph->rst = 0;
    tcph->psh = 0;
    tcph->ack = 0;
    tcph->urg = 0;
    tcph->window = htons (5840);    /* maximum allowed window size */
    tcph->check = 0;    //leave checksum 0 now, filled later by pseudo header
    tcph->urg_ptr = 0;

    //Now the TCP checksum
    psh.source_address = inet_addr(source_ip);
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data));

    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
    pseudogram = (char*)malloc(psize);
    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr) + strlen(data));
    //tcp CRC校验包括：12字节伪首部+tcp首部+数据包
    tcph->check = csum((unsigned short*)pseudogram,psize);

    //TP_HDRINCL to tell the kernel that headers are included in the pakcet
    int one = 1;
    const int *val = &one;
    
    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(int)) < 0)
    {
        perror("Error setting IP_HDRINCL\n");
        exit(0);
    }

    //loop if you want to flood
    while(1)
    {
        //send the packet
        if(sendto(s, datagram, iph->tot_len, 0, (struct sockaddr*)&sin, sizeof(sin)) < 0)
        {
            perror("send to failed\n");
        }
        else
        {
            printf("Packet Send Length:%d \n",iph->tot_len);
        }
        sleep(2);
    }
    
    return 0;
}

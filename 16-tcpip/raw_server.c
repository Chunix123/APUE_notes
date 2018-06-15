#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define MAX_BUFF 65535


struct sockaddr_in source,dest;

void ip_header(unsigned char* buffer, int buflen)
{
    struct iphdr *ip = (struct iphdr*)(buffer);
    int iphdrlen = ip->ihl*4;
    
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip->saddr;
    
    if(strcmp(inet_ntoa(source.sin_addr), "192.168.9.182") == 0)
        //return;
    
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip->daddr;

    printf( "\nIP Header\n");
    printf( "\t|-Version              : %d\n", (unsigned int)ip->version);
    printf( "\t|-Internet Header Length  : %d DWORDS or %d Bytes\n", (unsigned int)ip->ihl, ((unsigned int)(ip->ihl))*4);
    printf( "\t|-Type Of Service   : %d\n", (unsigned int)ip->tos);
    printf( "\t|-Total Length      : %d  Bytes\n", ntohs(ip->tot_len));
    printf( "\t|-Identification    : %d\n", ntohs(ip->id));
    printf( "\t|-Time To Live        : %d\n", (unsigned int)ip->ttl);
    printf( "\t|-Protocol         : %d\n", (unsigned int)ip->protocol);
    printf( "\t|-Header Checksum   : %d\n", ntohs(ip->check));
    printf( "\t|-Source IP         : %s\n", inet_ntoa(source.sin_addr));
    printf( "\t|-Destination IP    : %s\n", inet_ntoa(dest.sin_addr));
    printf( "Size =%d\n",buflen);
    
    char* data = buffer+sizeof(struct iphdr)+sizeof (struct tcphdr);
    printf( "\nData\n");
    int i;
    int remaining_data = buflen - ( sizeof(struct iphdr) + sizeof(struct tcphdr));
    struct tcphdr *tcp = (struct tcphdr*)buffer;
    
    printf( "Remaining %d \n",remaining_data);
    for(i=0;i<remaining_data;i++)
    {
        if(i!=0 && i%16==0)
            printf( "\n");
        printf( " %.2X ",data[i]);
    }

    printf( "\n");
    printf( "\t|-Data   :%s ",data);
}

int main()
{
    int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    
    if(rsfd < 0)
    {
        printf("Exit\n");
        perror("socket");
        exit(0);
    }
    
    struct iphdr *iph = (struct iphdr *) malloc(MAX_BUFF);
    struct sockaddr_in sin;
    socklen_t socklength;
    
    while(1)
    {
        bzero(iph, MAX_BUFF);
        int buflen = recvfrom(rsfd, iph, MAX_BUFF, 0, (struct sockaddr*)&sin, &socklength);
        printf("Below Len= %d\n", buflen);
        ip_header((char*)iph, buflen);
    }
    
    return 0;
}
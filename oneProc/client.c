/*************************************************************************
    > File Name: client.c
    > Author: kinght
    > Mail: zhjnight@163.com 
    > Created Time: Wed 20 Jun 2018 06:58:02 PM PDT
 ************************************************************************/

#include<stdio.h>
#include <sys/socket.h>   //connect,send,recv,setsockopt等
#include <sys/types.h>      

#include <netinet/in.h>     // sockaddr_in, "man 7 ip" ,htons
#include <poll.h>             //poll,pollfd
#include <arpa/inet.h>   //inet_addr,inet_aton
#include <unistd.h>        //read,write
#include <netdb.h>         //gethostbyname

//#include <error.h>         //perror
#include <stdio.h>
//#include <errno.h>         //errno

#include <string.h>          // memset
//#include <string>
//#include <iostream>
#include<stdlib.h>
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#define ERR_EXIT(m)\
    do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
}while(0)

int main(void)
{
    int sock;
    if((sock =socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
    {
        ERR_EXIT("ERROR");   
    }
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(sock,(struct sockaddr*)&servaddr,sizeof(servaddr))< 0)
    {
    ERR_EXIT("connect");
    }
    char sendbuf[1024]={0};
    char recvbuf[1024]={0};
    while(fgets(sendbuf,sizeof(sendbuf),stdin)!=NULL)
    {
    write(sock,sendbuf,strlen(sendbuf));
    read(sock,recvbuf,sizeof(recvbuf));
    fputs(recvbuf,stdout);
    memset(sendbuf,0,sizeof(sendbuf));
    memset(recvbuf,0,sizeof(recvbuf));
    }
    close(sock);
    return 0;
}

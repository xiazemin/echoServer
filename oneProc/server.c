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
#include <errno.h>         //errno

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
    int listenfd;
    if((listenfd =socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
    {
        ERR_EXIT("ERROR");
    }
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int on = 1;
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))< 0)
    {
    ERR_EXIT("SETSOCKOPT");
    }
    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))< 0)
    {
        ERR_EXIT("error");
    }
    if(listen(listenfd,SOMAXCONN)<0)
    {
        ERR_EXIT("listen");
    }
    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof(peeraddr);
    int conn;
    conn = accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen);
    if(conn < 0)
    {
    ERR_EXIT("accept");
    }
    printf("ip= %s,port = %d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
    char recvbuf[1024];
    while(1)
    {
    memset(recvbuf,0,sizeof(recvbuf));
    int ret = read(conn,recvbuf,sizeof(recvbuf));
    if(ret < 0)
        continue;

    write(conn,recvbuf,ret);
    printf(recvbuf,stdout);
    }   
    close(conn);
    close(listenfd);
    return 0;
}

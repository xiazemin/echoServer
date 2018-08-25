#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
//#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>  
#include <sys/wait.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <signal.h>
 
void sigChildFun(int sigNO)
{
    pid_t pid;
    int stat;
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0) // 循环收尸(僵尸进程), 此时waitpid不会阻塞
	{
		NULL;
	}
	
    return;
}
 
int main()
{
    struct sockaddr_in servAddr;
    memset(&servAddr,0,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    // servAddr.sin_addr.s_addr = INADDR_ANY;
    // servAddr.sin_port = htons(8765);
    servAddr.sin_port = htons(5188);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int iListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(iListenSocket, (struct sockaddr *)&servAddr, sizeof(servAddr));
    listen(iListenSocket,5);
 
    signal(SIGCHLD, sigChildFun);
 
    while(1)
    {
    	struct sockaddr_in clientAddr;
        socklen_t iSize = sizeof(clientAddr);
        memset(&clientAddr, 0, sizeof(clientAddr));
		
		int iConnSocket = accept(iListenSocket,(struct sockaddr*)&clientAddr, &iSize);
        if(iConnSocket < 0)
        {
            if(errno == EINTR || errno == ECONNABORTED)
        	{
            	continue;
        	}
            else
            {
                printf("accept error, server\n");
                return -1;
            }
        }
 
		int tmpPid = fork();
        if(tmpPid == 0)
        {
            close(iListenSocket); // 子进程让监听socket的计数减1, 并非直接关闭监听socket
            
		    char szBuf[1024] = {0};
			snprintf(szBuf, sizeof(szBuf), "server pid[%u], client ip[%s]", getpid(), inet_ntoa(clientAddr.sin_addr));
            write(iConnSocket, szBuf, strlen(szBuf) + 1);
 
			while(1)
			{
				if(read(iConnSocket, szBuf, 1) <= 0)
				{
					close(iConnSocket);  // 子进程让通信的socket计数减1
            		return -2;           // 子进程退出
				}
			}
			
            close(iConnSocket);  // 子进程让通信的socket计数减1
            return 0;            // 子进程退出
        }
        
        close(iConnSocket);      // 父进程让通信的socket计数减1
    }
 
	getchar();
	close(iListenSocket);        // 父进程让监听socket计数减1, 此时会关掉监听socket(因为之前子进程已经有此操作)
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 69
#define RRQ 1
#define WRQ 2
#define DATA 3
#define ACK 4
#define ERROR 5

int main(int argc,char** argv)
{
    struct sockaddr_in serveraddr;
    //int serverfd = socket(PF_INET,SOCK_STREAM,0);
    int serverfd = socket(PF_INET,SOCK_DGRAM,0);
    if(serverfd < 0)
    {
        perror("create socket error");
        exit(EXIT_FAILURE);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);

//    int on = 1;
//    setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,(const void*)&on,sizeof(on));

    if(bind(serverfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
    {
       perror("bind error");
       exit(EXIT_FAILURE);
    }

    struct sockaddr_in cliaddr = serveraddr;
    socklen_t len = sizeof(cliaddr);
    char buf[512];
    while(1)
    {
        memset(buf,0,sizeof(buf));
        int errno = recvfrom(serverfd,buf,sizeof(buf),0,(struct sockaddr*)&cliaddr,&len);
        if(errno <= 0)
        {
             perror("recvfrom error");
        }
        printf("recv msg: %s",buf);
        sendto(serverfd,buf,strlen(buf),0,(struct sockaddr*)&cliaddr,len);
    }

    return 0;
}

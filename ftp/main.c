#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9999

int main(int argc,char** argv)
{
	struct sockaddr_in serveraddr;
	int listenfd = socket(PF_INET,SOCK_STREAM,0);
	if(listenfd < 0)
	{
		perror("create socket error");
		exit(EXIT_FAILURE);
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);

	int on = 1;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void*)&on,sizeof(on));

	if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
	{
		perror("bind error");
		exit(EXIT_FAILURE);
	}
	if(listen(listenfd, 10) < 0)
	{
		perror("listen error");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		struct sockaddr_in cliaddr;
		socklen_t len = sizeof(cliaddr);
		int clifd = accept(listenfd,(struct sockaddr*)&cliaddr,&len);
		while(1)
		{
			char buf[255];
			memset(buf,0,sizeof(buf));
			//scanf("%s",buf);
			gets(buf);
			sprintf(buf,"%s\r\n",buf);
			write(clifd,buf,strlen(buf));
			read(clifd,buf,sizeof(buf));
		}
	}

	return 0;
}

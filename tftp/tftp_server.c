#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include <unistd.h>
#include "session.h"

#define PORT 69
#define RRQ 1
#define WRQ 2
#define DATA 3
#define ACK 4
#define ERROR 5


#define BUF_SIZE 600
#define FILE_NAME_BUF_SIZE 512


SESSION *handle_RRQ(struct sockaddr* addr, int addr_len,char *in_buf);
int handle_WRQ(char *in_buf);

char send_buf[BUF_SIZE];
FILE * file = NULL;

void send_error(int error_code,char *error_msg)
{
	
}
int make_data(char *dst_buf,short int block,char *src_data_buf,int data_len)
{
	int index = 0;	
	short opcode = htons(DATA);	
	memcpy(dst_buf+index,&opcode,sizeof(opcode));
	index += 2;

	short int net_block = htons(block);
	memcpy(dst_buf+index,&net_block,sizeof(net_block));
	index += 2;

	memcpy(dst_buf+index,src_data_buf,data_len);
	index += data_len;

	return index;
}

SESSION* create_new_session(FILE *file,struct sockaddr* addr, int addr_len)
{
	SESSION *sess = malloc(sizeof(SESSION));
	sess->sock_fd = socket(AF_INET,SOCK_DGRAM,0);
	sess->pFile = file;
	sess->cli_addr = malloc(addr_len);
	memcpy(sess->cli_addr, addr, addr_len);
	sess->cli_addr_len = addr_len;
	return sess;
}
void free_session(SESSION *sess)
{
	if(sess)
	{
		free(sess->cli_addr);
		free(sess);
	}
}
SESSION *handle_new_connection(struct sockaddr* addr, int addr_len, char *in_buf)
{
	int index = 0;
	SESSION* sess =NULL; 
	unsigned short req = ntohs(*(unsigned short *)&in_buf[0]);
	switch(req)
	{
	case RRQ:
		sess = handle_RRQ(addr,addr_len, in_buf);	
	case WRQ:
		handle_WRQ(in_buf);
		break;
	default:
		printf("warning: can't handle this req: %d\n",req);
	}
	return sess;
}
SESSION *handle_RRQ(struct sockaddr* addr, int addr_len,char *in_buf)
{
	char file_name[512] = {0};
	char mode[512] = {0};
	char *pIndex = &in_buf[2];
	char *pFile = file_name;
	char *pMode = mode;
	while(*pIndex != '\0')
	{
		*pFile++ = *pIndex++;	
	}
	pIndex++;
	while(*pIndex != '\0')
	{
		*pMode++ = *pIndex++;
	}

	printf("RRQ file_name: %s,name length = %d\n",file_name,(int)strlen(file_name));
	printf("RRQ mode: %s\n",mode);

	//FILE *file = fopen(file_name,"r");

	file = fopen(file_name,"r");
	if(NULL == file)
	{
		// TODO:send error	
        char *path = get_current_dir_name();
		printf("open %s error,cwd = %s\n",file_name,path);
        free(path);
		return NULL;
	}

	char buf[512] = {0};
	char out_buf[512] = {0};
	int read_len = fread(buf, 1, 512, file);
	int len = make_data(out_buf, 1, buf, read_len);
	
	SESSION* sess = create_new_session(file, addr, addr_len);
	
	sendto(sess->sock_fd,out_buf,len,0,sess->cli_addr,sess->cli_addr_len);
	return sess;
}

int handle_WRQ(char *in_buf)
{
	return 0;
}
int handle_ACK(char *in_buf,char *out_buf)
{
	if(!file)
	{
		return 0;
	}
	short int block = 0;
	block = ntohs(*(short int*)(in_buf+2));
	fseek(file,512*block,SEEK_SET);

	char buf[512] = {0};
	int read_len = fread(buf, 1, 512, file);
	if( read_len < 512)
	{
		printf("fclose\n");
		fclose(file);
		file = NULL;
	}

	int len = make_data(out_buf, block+1, buf, read_len);
	
	return len;
}
int handle_req(struct sockaddr* addr,int addr_len, char *in_buf,char *out_buf)
{
	int index = 0;
	unsigned short req = ntohs(*(unsigned short *)&in_buf[0]);
	switch(req)
	{
	case DATA:
		{
			short int seq=htons(*(unsigned short int*)&in_buf[1]);
			printf("DATA: %d\n",seq);
			break;
		}
	case ACK:
		{
			short seq=htons(*(unsigned short *)&in_buf[1]);
			index = handle_ACK(in_buf,out_buf);
			printf("ACK: %d\n",seq);
			break;
		}
	case ERROR:
		{
			unsigned short errcode=htons(*(unsigned short *)&in_buf[1]);
			printf("ERROR: %d %s\n",errcode,&in_buf[3]);
			break;
		}
		
	default:
		printf("unkown request: %d\n",req);
	}
	return index;

}

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
    char in_buf[512];
    char out_buf[600];

	int epollfd = epoll_create1(0);
	if (epollfd == -1) 
	{
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}

	#define MAX_EVENTS 10
	struct epoll_event ev,events[MAX_EVENTS];
	ev.events = EPOLLIN;
	ev.data.fd = serverfd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, serverfd, &ev))
	{
		perror("epoll_ctl: server fd");	
		exit(EXIT_FAILURE);
	}

	for (;;)
	{
		int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);	
		if (nfds == -1)
		{
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}

		for (int n = 0; n < nfds; ++n)
		{
			if (events[n].data.fd == serverfd)	
			{
				int errno = recvfrom(serverfd,in_buf,sizeof(in_buf),0,(struct sockaddr*)&cliaddr,&len);
				SESSION *sess = handle_new_connection((struct sockaddr*)&cliaddr, len, in_buf);
				if(sess)
				{
					ev.events = EPOLLIN;
					ev.data.fd = sess->sock_fd;
					if (epoll_ctl(epollfd, EPOLL_CTL_ADD, serverfd, &ev))
					{
						perror("epoll_ctl: server fd");	
						exit(EXIT_FAILURE);
					}
				}
			}
			else
			{
					
			}
		}
	}


    return 0;
}

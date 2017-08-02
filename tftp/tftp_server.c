#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>

#define PORT 69
#define RRQ 1
#define WRQ 2
#define DATA 3
#define ACK 4
#define ERROR 5


#define BUF_SIZE 600
char send_buf[BUF_SIZE];
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
int handle_RRQ(char *in_buf,char *out_buf)
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

	FILE *file = fopen(file_name,"r");
	if(NULL == file)
	{
		// TODO:send error	
        char *path = get_current_dir_name();
		printf("open %s error,cwd = %s\n",file_name,path);
        free(path);
		exit(0);
	}

	char buf[512] = {0};
	int read_len = fread(buf, 512, 1, file);
	int len = make_data(out_buf, 0, buf, read_len);
	fclose(file);
	
	return len;
}

int handle_WRQ(char *in_buf,char *out_buf)
{
	printf("WRQ: %s\n",&in_buf[1]);
	int index = 0;
	return index;
}
int handle_req(char *in_buf,char *out_buf)
{
	int index = 0;
	unsigned short req = ntohs(*(unsigned short *)&in_buf[0]);
	switch(req)
	{
	case RRQ:
		index = handle_RRQ(in_buf,out_buf);	
		break;
	case WRQ:
		index = handle_WRQ(in_buf,out_buf);
		break;
	case DATA:
		{
			short int seq=htons(*(unsigned short int*)&in_buf[1]);
			printf("DATA: %d\n",seq);
			break;
		}
	case ACK:
		{
			short seq=htons(*(unsigned short *)&in_buf[1]);
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
    while(1)
    {
        memset(in_buf,0,sizeof(in_buf));
        memset(out_buf,0,sizeof(out_buf));
        int errno = recvfrom(serverfd,in_buf,sizeof(in_buf),0,(struct sockaddr*)&cliaddr,&len);
        if(errno <= 0)
        {
             perror("recvfrom error");
        }
        //printf("recv msg: %s",buf);
		int out_len = handle_req(in_buf,out_buf);
        sendto(serverfd,out_buf,out_len,0,(struct sockaddr*)&cliaddr,len);
    }

    return 0;
}

#include <stdio.h>
#include <sys/socket.h>

typedef struct session
{
	int  sock_fd;
	FILE *pFile;
	struct sockaddr* cli_addr;
	int cli_addr_len;
}SESSION;

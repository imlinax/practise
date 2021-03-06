#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <map>
#include <queue>

#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

const int PORT = 7890;
const int MAX_EVENTS = 20;
int setfd_noblock(int fd)
{
	int fdflag = fcntl(fd, F_GETFL);
	if(fdflag == -1)
	{
		return -1;
	}
	return fcntl(fd, F_SETFL, fdflag | O_NONBLOCK);
}
class session
{
public:
    session() {};
    void push(std::string data)
    {
        data_buf_.push(data);
    }

private:
    std::queue<std::string> data_buf_;
};
int main(int argc, char** argv)
{
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);

	if(setfd_noblock(listenfd) == -1)
	{
		close(listenfd);
		perror("set listenfd to non-block error");
	}

	sockaddr_in srv_addr;
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(PORT);

	const int reuseaddr_flag = 1;
	if ( setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_flag, sizeof(int)) == -1 )
	{
		close(listenfd);
		perror("set socket option SO_REUSEADDR fail");
	}
	if( bind(listenfd, (sockaddr *)&srv_addr, sizeof(srv_addr)))
	{
		std::cerr << "bind error" << std::endl;
	}

	if( listen(listenfd, 5))
	{
		std::cerr << "listen error" << std::endl;
	}

	int epfd = epoll_create1(0);

	if (epfd == -1)
	{
		std::cerr << "create epoll error" << std::endl;
	}
	
	struct epoll_event ev, events[MAX_EVENTS];
	//ev.events = EPOLLIN | EPOLLET;
	ev.events = EPOLLIN;
	ev.data.fd = listenfd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) == -1)
	{
		std::cerr << "epoll ctl error"<< std::endl;
	}

	char readbuf[1024];
	int nread = 0;
    std::map<int, std::queue<std::string> > session_map; // fd -> data buffer
	for(;;)
	{
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, 1000);
		for(int i = 0; i < nfds; i++)
		{
			if( events[i].events & EPOLLIN)
			{
                // std::cout << "GET EPOLLIN" << std::endl;
				if(events[i].data.fd == listenfd)	
				{
					sockaddr_in cli_addr;
					socklen_t len = sizeof(cli_addr);
					int clientfd = accept(listenfd,(struct sockaddr *)&cli_addr, &len);	
					if ( clientfd > 0)
					{
						if( setfd_noblock(clientfd) == -1)
						{
							perror("set clifd nonblock error");
							continue;
						}
						ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
						ev.data.fd = clientfd ;
						epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &ev);
					}
					std::cout << "accept client" << std::endl;
				}
				else
				{
                    for(;;)
                    {
                        nread = read(events[i].data.fd, readbuf, 1024);
                        if(nread == -1)
                        {
                            if (errno == EAGAIN || errno == EWOULDBLOCK)
                            {
                                break;
                            }
                            else
                            {
                                // connection closed
                                perror("session closed: ");
                                session_map.erase(events[i].data.fd);

                            }
                        }
                        else if(nread == 0)
                        {
                            break;
                        }
                        else
                        {
                            int fd = events[i].data.fd;
                            if(session_map.find(fd) == session_map.end())
                            {
                                int nwrite = write(fd, readbuf, nread/2);
                                if(nwrite != nread)
                                {
                                    // std::cout << "push  half data" << std::endl;
                                    session_map[events[i].data.fd].push(std::string(readbuf+nwrite,nread - nwrite));
                                }
                            }
                            else
                            {
                                    // std::cout << "push  total data" << std::endl;
                                    session_map[events[i].data.fd].push(std::string(readbuf,nread));
                                
                            }
                        }
                    } 
				}
            }
            if (events[i].events & EPOLLOUT)
            {
                // std::cout << "get EPOLLOUT" << std::endl;
                if (session_map.find(events[i].data.fd) == session_map.end())
                {
                    continue;
                }
                for(;;)
                {
                    auto &out_data = session_map[events[i].data.fd].front();
                    int nwrite = write(events[i].data.fd, out_data.c_str(), out_data.size());
                    assert(nwrite <= 1024);
                    if(nwrite < out_data.size())
                    {
                        out_data.erase(0,nwrite);
                        break;
                    }
                    else
                    {
                        session_map[events[i].data.fd].pop();
                        break;
                    }
                }
            }
        }

	}

    std::cout << "Server done" << std::endl;
	return 0;
}

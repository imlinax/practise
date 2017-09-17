#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


const int PORT = 7890;
const int MSG_SIZE=1024;
const char MSG[MSG_SIZE] = {};

void read_cb(struct bufferevent *bev, void* ctx)
{
    struct evbuffer *input = bufferevent_get_input(bev);
    bufferevent_write_buffer(bev,input);
}
void event_cb(struct bufferevent *bev,short event, void* args)
{
   if (event & BEV_EVENT_EOF) 
   {
        std::cout << "Connection closed" << std::endl;
   }
   else if (event & BEV_EVENT_ERROR)
   {
        std::cout << "Some error occuerd" << std::endl;
   }
   else if (event & BEV_EVENT_CONNECTED)
   {
        std::cout << "Client has successfully connected" << std::endl;
        bufferevent_write(bev, MSG, MSG_SIZE);
   }
   else
   {
        abort();
   }

   // bufferevent_free(bev);
}
void listener_cb(struct evconnlistener * evlistener, evutil_socket_t fd, struct sockaddr * addr, int socklen, void *user_data)
{
    std::cout << "accept" << std::endl;
    struct event_base *base = static_cast<struct event_base*>(user_data);
    struct bufferevent *bev;

    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        std::cerr << "Error constructing bufferevent!" << std::endl;
    }

    bufferevent_setcb(bev, read_cb, NULL, NULL, NULL);
    bufferevent_enable(bev, EV_READ | EV_PERSIST);

}

class echo_client
{
public:
    echo_client(struct event_base *base, std::string server_ip, int port):base_(base)
    {
      server_addr_.sin_family = AF_INET;
      server_addr_.sin_port = htons(port);
      server_addr_.sin_addr.s_addr = inet_addr(server_ip.c_str()); 
    }
    void start()
    {
        bev_ = bufferevent_socket_new(base_, -1, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_socket_connect(bev_, (struct sockaddr*)&server_addr_, sizeof(server_addr_) );
        bufferevent_setcb(bev_, read_cb, NULL, event_cb, NULL);
        bufferevent_enable(bev_, EV_READ | EV_PERSIST);
    }
private:
    struct event_base *base_;
    struct sockaddr_in server_addr_;
    bufferevent *bev_;

};
int main(int argc, char** argv)
{
    // event2 init
    struct event_base *base;
    base = event_base_new();
    if (!base) 
    {
        std::cerr << "Could not initialize libevent!" << std::endl;
        return 1;
    }

    // event2 connect 
    echo_client client(base, "127.0.0.1", PORT);
    client.start();

    event_base_dispatch(base);

    // free 
    event_base_free(base);

    std::cout << "Done" << std::endl;
    return 0;
}

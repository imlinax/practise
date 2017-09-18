#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <memory>
#include <thread>

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

void start_thread()
{
    // event2 init
    struct event_base *base;
    base = event_base_new();
    if (!base) 
    {
        std::cerr << "Could not initialize libevent!" << std::endl;
        return;
    }

    // event2 connect 
    std::vector<std::shared_ptr<echo_client>> client_vec;
    for (int i = 0; i < 10; i++)
    {
        std::shared_ptr<echo_client> client_ptr(new echo_client(base, "127.0.0.1", PORT));
        client_vec.push_back(client_ptr);
    }

    for (auto client : client_vec)
    {
        client->start();
    }

    event_base_dispatch(base);

    // free 
    event_base_free(base);

}

int main(int argc, char** argv)
{
    std::thread thread1(start_thread);
    std::thread thread2(start_thread);
    thread1.join();
    thread2.join();
    std::cout << "Done" << std::endl;
    return 0;
}

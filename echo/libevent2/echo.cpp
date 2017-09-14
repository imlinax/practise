#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <sys/socket.h>


const int PORT = 7890;
void listener_cb(struct evconnlistener * evlistener, evutil_socket_t socket, struct sockaddr * addr, int socklen, void *ptr)
{
    std::cout << "accept" << std::endl;
}

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

    // event2 listen 
    struct evconnlistener *listener;
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    int flags = 0;
    listener = evconnlistener_new_bind(base, listener_cb, (void *)base, flags, -1, (struct sockaddr*)&sin, sizeof(sin) );
    if (!listener)
    {
        std::cerr << "Could not create a listener!" << std::endl;
        return 1;
    }

    event_base_dispatch(base);

    // free 
    evconnlistener_free(listener);
    event_base_free(base);

    std::cout << "Done" << std::endl;
    return 0;
}

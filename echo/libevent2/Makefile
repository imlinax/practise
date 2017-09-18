CXX=g++
CFLAGS +=-I./libevent2-dist/include 
CXXFLAGS +=-std=c++11 -I./libevent2-dist/include
LDFLAGS +=-L./libevent2-dist/lib
LIBS=-Wl,-Bstatic -levent_core -levent  -Wl,-Bdynamic -lpthread
.PHONY: all

all:echo_server echo_client

echo_server:echo_server.cpp 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

echo_client:echo_client.cpp 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@


clean:
	-rm -f echo_server
	-rm -f echo_client
	-rm -f *.o

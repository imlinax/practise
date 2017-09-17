CXX=clang++
CFLAGS +=-I./libevent2-dist/include 
CXXFLAGS +=-std=c++11 -I./libevent2-dist/include -pthread -static 
#CXXFLAGS +=-std=c++11 -I./libevent2-dist/include -pthread 
LDFLAGS +=-L./libevent2-dist/lib
LIBS+=-levent_core -levent -pthread
.PHONY: all

all:echo_server echo_client

echo_server:echo_server.cpp 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

echo_client:echo_client.cpp 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

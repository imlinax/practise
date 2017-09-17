CXX=clang++
CFLAGS +=-I./libevent2-dist/include 
CXXFLAGS +=-std=c++11 -I./libevent2-dist/include -static
LDFLAGS +=-L./libevent2-dist/lib
LIBS+=-levent_core -levent
echo_server:echo_server.cpp 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

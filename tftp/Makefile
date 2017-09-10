CC=clang
CXX=clang++
GTEST_INCLUDE_PATH=../gtest/include
GTEST_LIB_PATH=../gtest/lib/x86_64-linux-gnu
CFLAGS=-g -I$(GTEST_INCLUDE_PATH) 
CXXFLAGS=-g -I$(GTEST_INCLUDE_PATH) 
LDFLAGS=-L$(GTEST_LIB_PATH)
LIBS=-lgtest -lpthread

.PHONY: all

all:tftp_server test

tftp_server:tftp_server.c

test:list_test

list_test:list_test.o list.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS) 

list_test.o:list_test.cc

list.o:list.c


#	$(CXX) $^ -o $@ 
clean:
	-rm -f *.o
	-rm list_test
	-rm tftp_server

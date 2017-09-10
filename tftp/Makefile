CFLAGS=-g

.PHONY: all

all:tftp_server test

tftp_server:tftp_server.c

test:list_test

list_test:list_test.o list.o
	$(CXX) $^ -lgtest -lpthread -o $@ 
 

clean:
	-rm -f *.o
	-rm list_test
	-rm tftp_server

all:client server

init.o:init.c init.h
	g++ -g -c init.c init.h

response.o:response.c response.h init.h
	g++ -g -c response.c response.h

thread.o:thread.c thread.h
	g++ -g -c thread.c thread.h -lpthread

epoll.o:epoll.c epoll.h response.h thread.h
	g++ -g -c epoll.c epoll.h -lpthread

tcpserver.o:tcpserver.c
	g++ -g -c  tcpserver.c -lpthread

server:tcpserver.o response.o init.o thread.o epoll.o
	g++ -g -o server tcpserver.o response.o init.o thread.o epoll.o -lpthread

client:tcpclient.c
	g++ -g -o client tcpclient.c

clean:
	rm *.h.gch *.o server client

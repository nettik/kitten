all:client server

init.o:init.c
	g++ -g -c init.c

request_and_response.o:request_and_response.c
	g++ -g -c request_and_response.c

thread.o:thread.c
	g++ -g -c thread.c

epoll.o:epoll.c
	g++ -g -c epoll.c

tcpserver.o:tcpserver.c
	g++ -g -c  tcpserver.c

server:tcpserver.o request_and_response.o init.o thread.o epoll.o
	g++ -g -o server tcpserver.o request_and_response.o init.o thread.o epoll.o -lpthread

client:tcpclient.c
	g++ -g -o client tcpclient.c

clean:
	rm  *.o server client

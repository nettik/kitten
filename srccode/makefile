all:client server

init.o:init.c
	g++ -g -c -W -Wall init.c

status_code.o:status_code.c
	g++ -g -c -W -Wall status_code.c

request_and_response.o:request_and_response.c
	g++ -g -c -W -Wall request_and_response.c

parse_http_request.o:parse_http_request.c
	g++ -g -c -W -Wall parse_http_request.c

thread.o:thread.c
	g++ -g -c -W -Wall thread.c

epoll.o:epoll.c
	g++ -g -c -W -Wall epoll.c

tcpserver.o:tcpserver.c
	g++ -g -c -W -Wall tcpserver.c

server:tcpserver.o request_and_response.o init.o thread.o epoll.o parse_http_request.o status_code.o
	g++ -g -W -Wall -o server tcpserver.o request_and_response.o init.o thread.o epoll.o parse_http_request.o status_code.o -lpthread

client:tcpclient.c
	g++ -g -W -Wall -o client tcpclient.c

clean:
	rm  *.o server client

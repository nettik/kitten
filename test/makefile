all:client server

response.o:response.cpp response.h
	g++ -c  response.cpp response.h

tcpserver.o:tcpserver.cpp
	g++ -c  tcpserver.cpp -lpthread

server:tcpserver.o response.o
	g++ -g -o server tcpserver.o response.o -lpthread

client:tcpclient.cpp
	g++ -g -o client tcpclient.cpp

clean:
	rm *.h.gch *.o server client

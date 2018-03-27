all:client server

server:tcpserver.cpp
	g++ -g -o server tcpserver.cpp -lpthread

client:tcpclient.cpp
	g++ -g -o client tcpclient.cpp

clean:
	rm server client

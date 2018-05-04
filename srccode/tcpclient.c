#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#define MAX_SIZE 1024


typedef sockaddr SA;

int main()
{
	int sockfd;
	struct sockaddr_in seraddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_port = htons(4000);
	inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr);
	seraddr.sin_family = AF_INET;

	connect(sockfd, (SA*)&seraddr, sizeof(seraddr));

	char sendbuffer[MAX_SIZE];
	while (std::cin.getline(sendbuffer, sizeof(sendbuffer)))
	{
		if (strcmp(sendbuffer, "quit") == 0)
			break;

		send(sockfd, sendbuffer, sizeof(sendbuffer), 0);

		ssize_t n;
		char recvbuffer[MAX_SIZE];
		n = recv(sockfd, recvbuffer, sizeof(recvbuffer), 0);
		recvbuffer[n] = '\0';

		printf("echo from server : %s\n", recvbuffer);
	}
	shutdown(sockfd, SHUT_WR);
}

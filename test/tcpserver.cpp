#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAX_SIZE 1024

typedef sockaddr SA;

int main()
{
	int listenfd, connfd;
	struct sockaddr_in cliaddr, seraddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(4000);
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenfd, (SA*)&seraddr, sizeof(seraddr));

	listen(listenfd, 5);
	
	bzero(&cliaddr, sizeof(cliaddr));
	char buffer[MAX_SIZE];
	ssize_t n;
	for (;;)
	{
		socklen_t clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (SA*)&cliaddr, &clilen);

		while (n = recv(connfd, buffer, sizeof(buffer), MSG_WAITALL))
		{
			buffer[n] = '\0';
			printf("receive from client : %s\n", buffer);
			send(connfd, buffer, n, 0);
		}
		close(connfd);
	}
	close(listenfd);
}

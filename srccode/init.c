#include "init.h"

int socket_bind_listen()
{
	int listenfd;
	struct sockaddr_in seraddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_addr.s_addr = htonl(IPADDR);
	seraddr.sin_port = htons(PORT);
	seraddr.sin_family = AF_INET;

	bind(listenfd, (SA*)&seraddr, sizeof(seraddr));
	
	listen(listenfd, LIS_QUEUE_SIZE);

	return listenfd;
}

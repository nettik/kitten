#include <stdlib.h>   //malloc()
#include "response.h"
#include "init.h"
#include "thread.h"
#include "epoll.h"
#include <unordered_map>

#define MAX_SIZE 1024

typedef sockaddr SA;

using namespace std;


int main()
{
	int listenfd;
	//int* connfd;
	//struct sockaddr_in cliaddr;
	//pthread_t tid;

	listenfd = socket_bind_listen();
	
	unordered_map<int, struct task_queue> task;


	do_epoll(listenfd, &task);



	/*bzero(&cliaddr, sizeof(cliaddr));
	socklen_t clilen = sizeof(cliaddr);
	
	for (;;)
	{
		connfd = (int*)malloc(sizeof(int));
		*connfd = accept(listenfd, (SA*)&cliaddr, &clilen);
		pthread_create(&tid, NULL, &thread_work, (void*)connfd);
	}*/
	
	close(listenfd);
}

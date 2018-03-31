#include "response.h"
#include "init.h"
#include "thread.h"
#include "epoll.h"
#include <unordered_map>


using namespace std;

//pthread_mutex_t lock_task = PTHREAD_MUTEX_INITIALIZER;

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

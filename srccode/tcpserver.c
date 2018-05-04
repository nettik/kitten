#include "request_and_response.h"
#include "init.h"
#include "thread.h"
#include "epoll.h"

int main()
{
	//handle_sigpipe();

	int listenfd;

	if ((listenfd = socket_bind_listen()) < 0)
		perror("socket_bind_listen");
	
	struct thread_pool_info* pool = init_thread_pool(THREAD_NUM);

	do_epoll(listenfd, pool);
	
	destroy_thread_pool(pool);

	close(listenfd);
}

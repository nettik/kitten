#include "thread.h"

struct thread_pool_info threadinfo;

void* thread_work_recv(void* arg)
{
	int connfd = ((struct thread_parameter*)arg)->sockfd;
	int epollfd = ((struct thread_parameter*)arg)->epollfd;
	unordered_map<int, struct task_queue>* taskptr = ((struct thread_parameter*)arg)->taskptr;

	delete((struct thread_parameter*)arg);

	//pthread_detach(pthread_self());
	//string_echo(connfd);
	do_recv(epollfd, connfd, taskptr);
	return NULL;
}

void* thread_work_send(void* arg)
{	
	int connfd = ((struct thread_parameter*)arg)->sockfd;
	int epollfd = ((struct thread_parameter*)arg)->epollfd;
	unordered_map<int, struct task_queue>* taskptr = ((struct thread_parameter*)arg)->taskptr;

	delete((struct thread_parameter*)arg);

	//pthread_detach(pthread_self());

	do_send(epollfd, connfd, taskptr);
	return NULL;
}

void init_thread_pool(int thread_num = THREAD_NUM)
{
	threadinfo.thread_run = true;

	pthread_mutex_init(&threadinfo.task_mutex, NULL);
	pthread_cond_init(&threadinfo.task_cond, NULL);

	threadinfo.thread_num = thread_num;
	threadinfo.task_num = 0;
	
	threadinfo.threadid = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);
	
	for (int i = 0; i < thread_num; ++i)
		pthread_create(&threadinfo.threadid[i], NULL, pthread_work, NULL);
}

void destroy_thread_pool()
{
	threadinfo.thread_run = false;
	pthread_cond_broadcast(&threadinfo.task_cond);

	for (int i = 0; i < thread_num; ++i)
		pthread_join(threadinfo.threadid[i], NULL);

	free(threadinfo.threadid);

	pthread_mutex_destroy(&task_mutex);
	pthread_cond_destroy(&task_cond);

}

void thread_pool_add_task(int connfd, struct task_queue task)
{
	pthread_mutex_lock(&threadinfo.task_mutex);
	
	(*threadinfo.taskptr)[connfd] = task;
	++threadinfo.task_num;
	pthread_cond_signal(&threadinfo.task_cond);
	
	pthread_mutex_unlock(&threadinfo.task_mutex);
}

struct task_queue thread_pool_retrieve()
{
	
}

void thread_pool_do_task(struct task_queue task)
{

}

void thread_work(void* thread_para)
{

}

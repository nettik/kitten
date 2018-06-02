#ifndef THREAD_H
#define THREAD_H

#include "init.h"

struct task_para
{
	int epollfd;
	int connfd;
};

struct task_node
{
	struct task_para* para;
	void (*func)(struct task_para*);
	struct task_node* next;
};

struct thread_pool_info
{
	bool               thread_run;
	int                thread_num;
	int                task_num;
	pthread_t*         threadid;
	pthread_mutex_t    task_mutex;
	pthread_cond_t     task_cond;
	struct task_node*  head;
};

struct thread_pool_info* init_thread_pool(int thread_num = THREAD_NUM);

void destroy_thread_pool(struct thread_pool_info* pool);

void thread_pool_add_task(struct thread_pool_info* pool, struct task_para* arg, void (*func)(struct task_para*));

void* thread_pool_work(void* arg);

#endif

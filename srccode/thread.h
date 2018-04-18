#ifndef THREAD_H
#define THREAD_H

#include "init.h"
#include "response.h"

struct task_node
{
	void* arg;
	void (*func)(void*);
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

void thread_pool_add_task(struct thread_pool_info* pool, void* arg, void (*func)(void*));

void* thread_pool_work(void* arg);


void* thread_work_recv(void* arg);

void* thread_work_send(void* arg);

#endif

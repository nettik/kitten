#ifndef THREAD_H
#define THREAD_H

#include "init.h"
#include "response.h"


using namespace std;


struct thread_parameter
{
	int epollfd;
	int sockfd;
	unordered_map<int, struct task_queue>* taskptr;
};

struct thread_pool_info
{
	bool               thread_run;
	int                thread_num;
	int                task_num;
	pthread_t*         threadid;
	pthread_mutex_t    task_mutex;
	pthread_cond_t     task_cond;
	unordered_map<int, struct task_queue>* taskptr;
};

void init_thread_pool(int thread_num);

void destroy_thread_pool();

void thread_pool_add_task(unordered_map<int, struct task_queue>* taskptr);

struct task_queue thread_pool_retrieve_task();

void thread_pool_do_task(struct task_queue taskptr);

void thread_work(void* thread_para);


void* thread_work_recv(void* arg);

void* thread_work_send(void* arg);

#endif

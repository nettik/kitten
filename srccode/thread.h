#ifndef THREAD_H
#define THREAD_H

#include "init.h"

struct task_para                     //任务参数
{
	int epollfd;
	int connfd;
};

struct task_node
{
	struct task_para* para;           //任务参数（传递给func）
	void (*func)(struct task_para*);  //执行任务的函数
	struct task_node* next;
};

struct thread_pool_info               //线程池结构体
{
	bool               thread_run;
	int                thread_num;    //线程数量
	int                task_num;      //任务链表中，任务数量
	pthread_t*         threadid;
	pthread_mutex_t    task_mutex;
	pthread_cond_t     task_cond;
	struct task_node*  head;          //任务链表头节点
};

struct thread_pool_info* init_thread_pool(int thread_num = THREAD_NUM);   //创建并初始化线程池

void destroy_thread_pool(struct thread_pool_info* pool);                  //销毁线程池

void thread_pool_add_task(struct thread_pool_info* pool, struct task_para* arg, void (*func)(struct task_para*));  //向线程池中添加任务

void* thread_pool_work(void* arg);    //线程工作函数

#endif

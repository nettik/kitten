#include "thread.h"

struct thread_pool_info* init_thread_pool(int thread_num)
{
	struct thread_pool_info* pool = (struct thread_pool_info*)malloc(sizeof(thread_pool_info));
	
	pool->thread_run = true;
	pool->thread_num = thread_num;
	pool->task_num = 0;

	if (pthread_mutex_init(&(pool->task_mutex), NULL) != 0)
		perror("pthread_mutex_init");
	if (pthread_cond_init(&(pool->task_cond), NULL) != 0)
		perror("pthread_cond_init");

	pool->head = (struct task_node*)malloc(sizeof(task_node));
	pool->head->para = NULL;
	pool->head->func = NULL;
	pool->head->next = NULL;

	pool->threadid = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);	
	for (int i = 0; i < thread_num; ++i)
		pthread_create(&(pool->threadid[i]), NULL, &thread_pool_work, (void*)pool);

	return pool;
}

static void destroy_task_node(struct thread_pool_info* pool)
{
	struct task_node* temp;
	while (pool->head->next)
	{
		temp = pool->head->next;
		pool->head->next = pool->head->next->next;
		free(temp->para);
		free(temp);
	}
	free(pool->head);
}


void destroy_thread_pool(struct thread_pool_info* pool)
{
	pool->thread_run = false;
	pthread_cond_broadcast(&(pool->task_cond));

	for (int i = 0; i < pool->thread_num; ++i)
		pthread_join(pool->threadid[i], NULL);
	
	free(pool->threadid);

	if (pthread_mutex_destroy(&(pool->task_mutex)) != 0)
		perror("pthread_mutex_destroy");
	if (pthread_cond_destroy(&(pool->task_cond)) != 0)
		perror("pthread_cond_destroy");

	destroy_task_node(pool);
	free(pool);
}

void thread_pool_add_task(struct thread_pool_info* pool, struct task_para* arg, void (*func)(struct task_para*))
{
	pthread_mutex_lock(&(pool->task_mutex));
	
	struct task_node* task = (struct task_node*)malloc(sizeof(task_node));
	task->para = (struct task_para*)malloc(sizeof(task_para));
	task->para->epollfd = arg->epollfd;
	task->para->connfd = arg->connfd;
	task->func = func;
	
	task->next = pool->head->next;
	pool->head->next = task;
	(pool->task_num)++;

	pthread_cond_signal(&(pool->task_cond));
		
	pthread_mutex_unlock(&(pool->task_mutex));
}

void* thread_pool_work(void* arg)  //arg表示线程池pool
{
	//handle_sigpipe();

	struct thread_pool_info* pool = (struct thread_pool_info*)arg;
	struct task_node* task;
	while (pool->thread_run)
	{
		pthread_mutex_lock(&(pool->task_mutex));
		while ((pool->task_num) == 0)
			pthread_cond_wait(&(pool->task_cond), &(pool->task_mutex));
		
		task = pool->head->next;
		if (task == NULL)
		{
			pthread_mutex_unlock(&(pool->task_mutex));
			continue;
		}
		pool->head->next = task->next;
		--(pool->task_num);
		pthread_mutex_unlock(&(pool->task_mutex));

		(*(task->func))(task->para);
		free(task->para);
		free(task);
	}
	return NULL;
}

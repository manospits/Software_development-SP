#include "jobscheduler.h"
#include "error.h"
#include <stdio.h>

struct JobScheduler
{
    int number_of_threads;
	pthread_t *thread_pool;
	pthread_mutex_t mtx_for_queue;
	pthread_cond_t cond_for_queue;
    //queue
};

struct thread_params
{
    pGraph graph;
};

void * worker_thread_function(void *params);

pJobScheduler initialize_scheduler(int execution_threads, pGraph graph, struct thread_params *params)
{   //  thread_params passed as argument instead of being local variable, to avoid possible segmentation fauls
    //  (i.e. initialize_scheduler() returning before last thread reads all parameters.)
    //  This could have been avoided with mutexes, but this way seems simpler

    // a sanity check
    if (execution_threads <= 0)
    {
        error_val = JOBSCHEDULER_INIT_INVALID_NUM_OF_THREADS;
        return NULL;
    }
    int i, ret_val;
    // create Job Scheduler
    pJobScheduler newJS;
    if ((newJS = malloc(sizeof(struct JobScheduler))) == NULL)
    {
        error_val = JOBSCHEDULER_INIT_INITIAL_MALLOC_FAIL;
        return NULL;
    }
    // initialize its fields
    newJS->number_of_threads = execution_threads;
    if ((newJS->thread_pool = malloc(execution_threads*sizeof(pthread_t))) == NULL)
    {
        error_val = JOBSCHEDULER_INIT_THREAD_POOL_MALLOC_FAIL;
        free(newJS);
        return NULL;
    }
    pthread_mutex_init(&(newJS->mtx_for_queue), NULL);
    pthread_cond_init(&(newJS->cond_for_queue), NULL);
    // initialize queue
    //
    // initialize thread parameters
    params->graph = graph;
    // create threads
    for (i = 0 ; i < execution_threads ; ++i)
	{
		if ((ret_val = pthread_create(&(newJS->thread_pool[i]), NULL, worker_thread_function, (void *)params)))
		{
		    fprintf(stderr, "pthread_create: %s\n", strerror(ret_val));
			error_val = JOBSCHEDULER_INIT_PTHREAD_CREATE_FAIL;
			return NULL;
		}
	}
    return newJS;
}

void submit_job(pJobScheduler scheduler, Job* j)
{

}

void execute_all_jobs(pJobScheduler scheduler)
{

}

void wait_all_tasks_finish(pJobScheduler scheduler)
{

}

void destroy_scheduler(pJobScheduler scheduler)
{
    // send signal to terminate threads
    // wait for threads to join
    pthread_mutex_destroy(&(scheduler->mtx_for_queue));
    pthread_cond_destroy(&(scheduler->cond_for_queue));
    free(scheduler->thread_pool);
    free(scheduler);
}

void * worker_thread_function(void *params)
{
    pthread_exit(NULL);
}

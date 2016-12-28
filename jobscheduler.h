#ifndef _JOBSCHEDULER_H_
#define _JOBSCHEDULER_H_

#include <pthread.h>
#include <stdint.h>
#include "graph.h"

typedef struct JobScheduler * pJobScheduler;
/*
struct queue_t
{
	int *queue;
	int queue_size;
	int start;
	int end;
	int count;
	pthread_mutex_t mtx;
	pthread_cond_t cond_nonempty;
	pthread_cond_t cond_nonfull;
};
*/

// replaced by query
struct Job
{
    char command;
    uint32_t from;
    uint32_t to;
};

pJobScheduler initialize_scheduler(int execution_threads, pGraph graph, struct thread_params *params);

void submit_job(pJobScheduler sch, uint32_t query_id, uint32_t from, uint32_t to, long unsigned int version);

void execute_all_jobs(pJobScheduler);

void wait_all_tasks_finish(pJobScheduler, int num_of_threads); //waits all submitted tasks to finish

void destroy_scheduler(pJobScheduler);

#endif // _JOBSCHEDULER_H_

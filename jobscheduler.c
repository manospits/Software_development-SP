#include "jobscheduler.h"
#include "error.h"
#include "queries.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

struct JobScheduler
{
    int number_of_threads;
    int finished_threads;
    int execution;
	pthread_t *thread_pool;
    qphead queue;
	pthread_mutex_t mtx_for_queue;
	pthread_cond_t cond_empty_queue;
	pthread_mutex_t sync_mtx;
	pthread_cond_t sync_cond;
};

struct thread_params
{
    pJobScheduler scheduler;
    pGraph graph;
    int count;
    int **result_array;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
};

void * worker_thread_function(void *params);

pJobScheduler initialize_scheduler(int execution_threads, pGraph graph, int **result_array)
{
    // a sanity check
    if (execution_threads <= 0)
    {
        error_val = JOBSCHEDULER_INIT_INVALID_NUM_OF_THREADS;
        return NULL;
    }
    int i, ret_val;
	struct thread_params params;
    // create Job Scheduler
    pJobScheduler newJS;
    if ((newJS = malloc(sizeof(struct JobScheduler))) == NULL)
    {
        error_val = JOBSCHEDULER_INIT_INITIAL_MALLOC_FAIL;
        return NULL;
    }
    // initialize its fields
    newJS->number_of_threads = execution_threads;
    newJS->finished_threads = 0;
    if ((newJS->thread_pool = malloc(execution_threads*sizeof(pthread_t))) == NULL)
    {
        error_val = JOBSCHEDULER_INIT_THREAD_POOL_MALLOC_FAIL;
        free(newJS);
        return NULL;
    }
    // initialize queue
    if ((newJS->queue = q_cr_list()) == NULL)
    {
        print_error();
        error_val = JOBSCHEDULER_INIT_QUEUE_CREATION_FAIL;
        free(newJS->thread_pool);
        free(newJS);
        return NULL;
    }
    pthread_mutex_init(&(newJS->mtx_for_queue), NULL);
    pthread_cond_init(&(newJS->cond_empty_queue), NULL);
    pthread_mutex_init(&(newJS->sync_mtx), NULL);
    pthread_cond_init(&(newJS->sync_cond), NULL);
    // initialize thread parameters
    params.scheduler = newJS;
    params.graph = graph;
    params.count = 0;
    params.result_array = result_array;
    pthread_mutex_init(&params.mtx, NULL);
    pthread_cond_init(&params.cond, NULL);
    // create threads
    for (i = 0 ; i < execution_threads ; ++i)
	{
		if ((ret_val = pthread_create(&(newJS->thread_pool[i]), NULL, worker_thread_function, (void *)&params)))
		{
		    fprintf(stderr, "pthread_create: %s\n", strerror(ret_val));
			error_val = JOBSCHEDULER_INIT_PTHREAD_CREATE_FAIL;
			return NULL;
		}
	}
    newJS->execution=0;
	// wait for all threads to take their parameters
	pthread_mutex_lock(&params.mtx);
	while (params.count != execution_threads)
        pthread_cond_wait(&params.cond, &params.mtx);
    // all threads read their parameters, destroy mutex & cond var and exit
    pthread_mutex_destroy(&params.mtx);
    pthread_cond_destroy(&params.cond);
    return newJS;
}

void submit_job(pJobScheduler scheduler, uint32_t query_id, uint32_t from, uint32_t to, long unsigned int version)
{
    if (scheduler != NULL)
    {
        if (q_insert_back(scheduler->queue, query_id, from, to, version))
        {
            print_error();
            error_val = JOBSCHEDULER_SUBMIT_INSERT_TO_QUEUE_FAIL;
        }
    }
}

void execute_all_jobs(pJobScheduler scheduler)
{
    /*printf("execution started , queue size %d \n",q_get_size(scheduler->queue));*/
    scheduler->execution=1;
    if (scheduler != NULL) pthread_cond_broadcast(&scheduler->cond_empty_queue);
}

void wait_all_tasks_finish(pJobScheduler scheduler, uint32_t num_of_threads)
{
    /*printf("waiting for %d threads\n",num_of_threads);*/
    if (scheduler != NULL)
    {
        pthread_mutex_lock(&scheduler->sync_mtx);
        while (scheduler->finished_threads != num_of_threads)
            pthread_cond_wait(&scheduler->sync_cond, &scheduler->sync_mtx);
        scheduler->finished_threads = 0;
        scheduler->execution = 0;
        pthread_mutex_unlock(&scheduler->sync_mtx);
    }
}

void destroy_scheduler(pJobScheduler scheduler)
{
    if (scheduler != NULL)
    {
        int i;
        // send signal to terminate threads
        for (i = 0 ; i < scheduler->number_of_threads ; ++i)
            submit_job(scheduler, 0, 0, 0, 0);
        execute_all_jobs(scheduler);
        // wait for threads to join
        for (i = 0 ; i < scheduler->number_of_threads ; ++i)
            pthread_join(scheduler->thread_pool[i], NULL);
        pthread_mutex_destroy(&(scheduler->mtx_for_queue));
        pthread_cond_destroy(&(scheduler->cond_empty_queue));
        q_ds_list(scheduler->queue);
        free(scheduler->thread_pool);
        free(scheduler);
    }
}

void * worker_thread_function(void *params)
{
    int **result_array;
    // read thread parameters
    pGraph graph = ((struct thread_params *)params)->graph;
    pJobScheduler scheduler = ((struct thread_params *)params)->scheduler;
    result_array = ((struct thread_params *)params)->result_array;
    pthread_mutex_lock(&((struct thread_params *)params)->mtx);
    ((struct thread_params *)params)->count++;
    pthread_cond_signal(&((struct thread_params *)params)->cond);
    pthread_mutex_unlock(&((struct thread_params *)params)->mtx);
    // other declarations/initializations
    qpnode temp;
    int result;
    phead open_list[2];
    pvis visited;
    if ((open_list[0] = cr_list()) == NULL)
    {
        print_error();
        fprintf(stderr, "Error creating list inside thread\n");
        exit(-1);
    }
    if ((open_list[1] = cr_list()) == NULL)
    {
        print_error();
        fprintf(stderr, "Error creating list inside thread\n");
        exit(-1);
    }
    if ((visited = create_visited(get_index_size(ret_inIndex(graph)))) == NULL)
    {
        print_error();
        fprintf(stderr, "Error creating list inside thread\n");
        exit(-1);
    }
    // main routine:
    while(1)
    {
        // wait to get an item from the queue
        pthread_mutex_lock(&scheduler->mtx_for_queue);
        while (!q_get_size(scheduler->queue) ||!scheduler->execution)
        {
            pthread_cond_wait(&scheduler->cond_empty_queue, &scheduler->mtx_for_queue);
        }
        //get item from queue
        temp = q_peek(scheduler->queue);
        q_pop_front(scheduler->queue);
        pthread_cond_signal(&scheduler->cond_empty_queue);
        pthread_mutex_unlock(&scheduler->mtx_for_queue);
        // process job
        // check if job signals thread to exit
        if (!temp->version)break;
        // otherwise, execute the query
        /*printf("asked:%d \n", temp->query_id);*/
        result = gFindShortestPath_t(graph, temp->nodea, temp->nodeb, open_list, visited, temp->version);
        /*printf("query:%d finished\n", temp->query_id);*/
        // output the result to the appropriate position in the array
        if (result >= 0)
            (*result_array)[temp->query_id] = result;
        else if (result == GRAPH_SEARCH_PATH_NOT_FOUND)
            (*result_array)[temp->query_id] = -1;
        else
        {
            printf("%d\n",result);
            print_error();
            fprintf(stderr, "An error occurred during graph search. Exiting...\n");
            exit(-1);
        }
        // job processing finished, inform the scheduler
        pthread_mutex_lock(&scheduler->sync_mtx);
        scheduler->finished_threads++;
        pthread_cond_signal(&scheduler->sync_cond);
        pthread_mutex_unlock(&scheduler->sync_mtx);
    }
    ds_list(open_list[0]);
    ds_list(open_list[1]);
    v_ds_visited(visited);
    pthread_exit(NULL);
}

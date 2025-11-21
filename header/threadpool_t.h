#ifndef THREADPOOL_T_H
#define THREADPOOL_T_H

#include <unistd.h>

//required dependecies
#include "queue_t.h"
#ifndef QUEUE_T_H
#error "queue is required"
#endif

typedef struct threadpool_t {
    int threads_amount;
    int isActive;
    pthread_t *threads;
    queue_t queue;
    pthread_mutex_t lock;
    pthread_cond_t signal;

} threadpool_t;

typedef struct threadjob_t {
    void * data;
    void * (*fptr)(void*);
} threadjob_t;

// CHANGED: Returns a pointer now
threadpool_t* threadpool_init(int); 
void threadpool_create_work(threadpool_t*, void*, void*(*fptr)(void*));
void* _threadroutine(void*);

#ifndef THREADPOOL_T_IMPLEMENTATION_H
#define THREADPOOL_T_IMPLEMENTATION_H

#if defined(_POSIX_VERSION)
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#else
#error "System is not posix compatible"
#endif

threadpool_t* threadpool_init(int _thread_amount) 
{
    assert(_thread_amount > 0);
    
    threadpool_t *threadpool = (threadpool_t*)malloc(sizeof(threadpool_t));
    if (threadpool == NULL) {
        perror("Failed to allocate threadpool");
        exit(EXIT_FAILURE);
    }

    threadpool->threads_amount = _thread_amount;
    threadpool->isActive = 1; 

    pthread_mutex_init(&threadpool->lock, NULL);
    pthread_cond_init(&threadpool->signal, NULL);
    threadpool->queue = queue_init();


    threadpool->threads = (pthread_t*)malloc(sizeof(pthread_t) * _thread_amount);
    assert(threadpool->threads != NULL);

    for (int i = 0; i < _thread_amount; i ++)
    {

        if (pthread_create(&threadpool->threads[i], NULL, _threadroutine, threadpool) != 0)
        {
            perror("ThreadPool: Thread was not created\n");
            exit(EXIT_FAILURE);
        }
    }

    return threadpool; 
}

void threadpool_create_work(threadpool_t * threadpool, void * arg, void*(*_fptr)(void*))
{
    pthread_mutex_lock(&threadpool->lock);

    threadjob_t * job = (threadjob_t*)malloc(sizeof(threadjob_t));
    assert(job != NULL);
    job->data = arg;
    job->fptr = _fptr;

    node_t * node = create_node(job);
    assert(node != NULL);

    queue_enqueue(&threadpool->queue, node);
    pthread_cond_signal(&threadpool->signal);
    pthread_mutex_unlock(&threadpool->lock);
}

void* _threadroutine (void * arg) 
{
    assert(arg != NULL);

    threadpool_t *threadpool = (threadpool_t*)arg;
    node_t *node = NULL;
    threadjob_t *job = NULL;
    while (1) 
    {   
        pthread_mutex_lock(&threadpool->lock);

        while(threadpool->queue.length <= 0) 
        {
            pthread_cond_wait(&threadpool->signal, &threadpool->lock);
        }

        node = queue_dequeue(&threadpool->queue);
        job = (threadjob_t*)node->data;

        pthread_mutex_unlock(&threadpool->lock);
        
        job->fptr(job->data);
        free(node);
        free(job);

    }
    
    return NULL;
}

#endif
#endif //thread pool
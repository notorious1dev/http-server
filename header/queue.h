#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>

typedef struct work {
    int fd;
    struct work *next;
} work_t;

typedef struct queue {
    work_t *head;
    work_t *tail;
    size_t size;
} queue_t;

// konstruktor kolejki
queue_t *queue_create();

// operacje na kolejce
void queue_push(queue_t *q, work_t *item);
work_t *queue_pop(queue_t *q);

#endif

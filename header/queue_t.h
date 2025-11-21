#ifndef QUEUE_T_H 
#define QUEUE_T_H
#include <stddef.h>

typedef struct node_t {
    void *data;
    struct node_t *next;
} node_t;

typedef struct queue_t
{
    node_t *head;
    node_t *tail;
    size_t length;
} queue_t;

queue_t queue_init();
node_t * create_node(void *input_data);
void queue_enqueue(queue_t *queue, node_t *node);
node_t * queue_dequeue(queue_t *queue);
node_t * queue_peek(queue_t *queue);
void queue_clear(queue_t *queue);
void queue_free(queue_t *queue);

#ifndef QUEUE_T_IMPLEMENTATION_H
#define QUEUE_T_IMPLEMENTATION_H 
#include <stdlib.h>
#include <assert.h>

queue_t queue_init() {
    queue_t queue = {0};
    queue.length = 0;
    queue.head = NULL;
    queue.tail = NULL;

    return queue;
}

node_t * create_node(void *input_data) {
    assert(input_data != NULL);
    
    node_t *node = (node_t*)malloc(sizeof(node_t));
    assert(node != NULL);

    node->next = NULL;
    node->data = input_data;

    return node;
}

void queue_enqueue(queue_t *queue, node_t * node) {
    assert(queue != NULL && node != NULL);

    if (queue->head == NULL) {
        queue->head = node;
        queue->tail = node;
    } 
    else
    {
        queue->tail->next = node;
        queue->tail = node;
    }

    queue->length += 1;
}

node_t * queue_dequeue(queue_t *queue)
{
    assert(queue != NULL);
    if (queue->head == NULL) { return NULL; }

    node_t *current_head = queue->head;
    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    
    queue->length -= 1;
    return current_head;
}

node_t * queue_peek(queue_t *queue) 
{
    assert(queue != NULL);
    return queue->head;
}

void queue_clear(queue_t *queue) 
{
    assert(queue != NULL);
    if (queue->head == NULL) return;

    node_t * current = queue->head;
    node_t * next = NULL;

    while(current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    queue->length = 0;
    queue->head = NULL;
    queue->tail = NULL;
}

void queue_free(queue_t *queue)
{
    assert(queue != NULL);
    queue_clear(queue);
    free(queue);
}

#endif //implementation
#endif //queue
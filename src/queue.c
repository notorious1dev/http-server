#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

queue_t *queue_create() {
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        printf("Error: queue malloc failed\n");
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void queue_push(queue_t *q, work_t *item) {
    if (!q || !item) return;

    item->next = NULL;

    if (q->tail == NULL) {
        q->head = item;
        q->tail = item;
    } else {
        q->tail->next = item;
        q->tail = item;
    }

    q->size++;
}

work_t *queue_pop(queue_t *q) {
    if (!q || q->head == NULL)
        return NULL;

    work_t *item = q->head;
    q->head = q->head->next;

    if (q->head == NULL)
        q->tail = NULL;

    q->size--;
    item->next = NULL;

    return item;
}

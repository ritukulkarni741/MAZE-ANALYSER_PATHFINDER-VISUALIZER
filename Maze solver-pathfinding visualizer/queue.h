#ifndef QUEUE_H
#define QUEUE_H

#include "main.h"

#define QUEUE_CAPACITY (MAX_ROWS * MAX_COLS)

typedef struct {
    Position items[QUEUE_CAPACITY];
    int front;
    int rear;
    int count;
} Queue;

void initQueue(Queue *queue);
bool isQueueEmpty(const Queue *queue);
bool enqueue(Queue *queue, Position value);
bool dequeue(Queue *queue, Position *value);

#endif

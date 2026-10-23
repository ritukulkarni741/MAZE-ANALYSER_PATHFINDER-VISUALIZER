#include "queue.h"

void initQueue(Queue *queue)
{
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
}

bool isQueueEmpty(const Queue *queue)
{
    return queue->count == 0;
}

bool enqueue(Queue *queue, Position value)
{
    if (queue->count == QUEUE_CAPACITY) {
        return false;
    }

    queue->items[queue->rear] = value;
    queue->rear = (queue->rear + 1) % QUEUE_CAPACITY;
    queue->count++;
    return true;
}

bool dequeue(Queue *queue, Position *value)
{
    if (isQueueEmpty(queue)) {
        return false;
    }

    *value = queue->items[queue->front];
    queue->front = (queue->front + 1) % QUEUE_CAPACITY;
    queue->count--;
    return true;
}

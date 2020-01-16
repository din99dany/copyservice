#include "queue.h"

void push(struct job x, struct queue* Q )
{
    (Q->last)++;
    Q->elem[(Q->last)] = x;
}

struct job front( struct queue* Q )
{
    return (*Q).elem[Q->first];
}

void pop( struct queue* Q )
{
    (Q->first)++;
}

int empty( struct queue* Q)
{
    return (Q->last) < (Q->first);
}
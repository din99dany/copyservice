#include <job.h>

struct queue
{
    struct job elem[10000];
    int first, last;
};  

void push(struct job x, struct queue* Q );
struct job front( struct queue* Q );
void pop( struct queue* Q );
int empty( struct queue* Q);

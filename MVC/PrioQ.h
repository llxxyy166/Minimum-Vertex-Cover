#ifndef __PRIOQ_H__
#define __PRIOQ_H__

/* Insert function prototypes and anything else you may require here */
typedef struct PriorityQueue *PQueue;

// function prototype: PriorityQueueCreate
PQueue PriorityQueueCreate();

// function prototype: PriorityQueueDestroy
void PriorityQueueDestroy(PQueue p);

// function prototype: PriorityQueueAddItem
int PriorityQueueAddItem(PQueue p, double priority,void *data);

// function prototype: PriorityQueueRemoveMinItem
void* PriorityQueueRemoveMinItem(PQueue p);

int PriorityQueueSize(PQueue p);//

#endif

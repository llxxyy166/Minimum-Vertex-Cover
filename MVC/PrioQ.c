#include <stdio.h>
#include <stdlib.h>
#include "PrioQ.h"

/* Fill in the functions here */
typedef struct Element *PQElement;
struct Element{
    double priority;
    void *data;
};

struct PriorityQueue{
    unsigned int size;//size
    unsigned int max; //maximum size
    PQElement queue;  //the array with type element
};
// function: PriorityQueueCreate
PQueue PriorityQueueCreate(){
    PQueue pq=(PQueue)malloc(sizeof(struct PriorityQueue)); //allocate space for queue
    if(pq!=NULL){
        pq->queue=(PQElement)malloc(sizeof(struct Element)*10); //allocate space for array, initalize it with size 10
        if(pq->queue!=NULL){
            pq->size=0;
            pq->max=10;
            return pq;
        }
        free(pq);
    }
    printf("Out of Space!\n");
    return NULL;
}
// function: PriorityQueueDestroy
void PriorityQueueDestroy(PQueue p){
    if(p==NULL){
        return;
    }
    free(p->queue);
    free(p);
}
// function: PriorityQueueAddItem
int PriorityQueueAddItem(PQueue p, double priority,void *data){
    if(p->size==p->max){
        p->queue=(PQElement)realloc(p->queue,sizeof(struct Element)*2*(p->max)); //if it is already full, reallocate memory
        p->max++;
    }
    int i;
    for(i=p->size;i>0&&p->queue[(i-1)/2].priority>priority;i=(i-1)/2){
        p->queue[i]=p->queue[(i-1)/2];
    }                                                             //find the position for insertion
    p->queue[i].data=data;
    p->queue[i].priority=priority;
    p->size++;
    return 0;
}
// function: PriorityQueueRemoveMinItem
void* PriorityQueueRemoveMinItem(PQueue p){
    if(p->size==0){
        printf("Empty queue!\n");
        return NULL;
    }
    void *data;
    data=p->queue[0].data;
    int s,i,child;
    s=--p->size;
    PQElement temp;
    temp=&p->queue[s];
    i=0;child=1;
    while(child<s){
        if(child<s-1&&p->queue[child].priority>p->queue[child+1].priority){
            child++;
        }  //find a smaller child node
        if(temp->priority>p->queue[child].priority){
            p->queue[i]=p->queue[child];
            i=child;
            child=2*i+1;
        }  //find the position
        else break;
    }
    p->queue[i]=*temp;
    return data;
}

int PriorityQueueSize(PQueue p){
    return p->size;
}
//
//  Approx.c
//  MVC
//
//  Created by xinye lei on 15/12/4.
//  Copyright © 2015年 xinye lei. All rights reserved.
//

#include "Approx.h"
#include "PrioQ.h"
#include <stdlib.h>

/*
 ***************************** Approximation algorithm ********************************************
 */
typedef struct MAX_DEGREE_GREEDY {
    unsigned node;
    unsigned degree;
}MDG;

int *mark;
int MDG_SOLUTION(Graph *g, FILE *fp) {  //Maximum Degree Greedy
    //size_t edgeSize = getEdge(g);
    fprintf(fp, "a\n");
    size_t nodeSize = getNumNodes(g);
    mark = malloc(sizeof(int) * (nodeSize + 1));
    PQueue pq = PriorityQueueCreate();
    MDG *mdg = malloc(sizeof(MDG) * (nodeSize+1));
    MDG *mdg1 = malloc(sizeof(MDG) * (nodeSize+1));
    for (int i = 1; i < nodeSize + 1; i++) {
        mdg[i].node = i;
        mdg[i].degree = getDegree(g, i);
        mark[i] = 0;
        PriorityQueueAddItem(pq, mdg[i].degree, mdg + i);
    }
    for (int i = 1; i < nodeSize + 1; i++) {
        MDG *temp = PriorityQueueRemoveMinItem(pq);
        mdg1[nodeSize+1-i].degree = temp->degree;
        mdg1[nodeSize+1-i].node = temp->node;
    }
    int vcSize = 0;
    for(int i = 1; i < nodeSize + 1; i++) {
        if (mark[mdg1[i].node]) {
            continue;
        }
        mark[mdg1[i].node] = 1;
        for (unsigned neigh = neigh_first(g,mdg1[i].node); !neigh_done(g); neigh = neigh_next(g)) {
            if (!mark[neigh]) {
                vcSize++;
                fprintf(fp, "%d,",mdg1[i].node);
                break;
            }
        }
    }
    PriorityQueueDestroy(pq);
    rewind(fp);
    fprintf(fp, "%d\n",vcSize);
    free(mdg);
    free(mdg1);
    return vcSize;
}
typedef struct Edge {
    unsigned from;
    unsigned to;
}Edge;
int RPE_SOLUTION(Graph *g) { //Random Pick Edge
    size_t edgeSize = getEdge(g);
    size_t nodeSize = getNumNodes(g);
    Edge* edge = malloc(sizeof(Edge)*2*edgeSize);
    PQueue pq = PriorityQueueCreate();
    int k = 0;
    int vcSize = 0;
    for (int i = 1; i < nodeSize + 1; i++) {
        for (unsigned neigh = neigh_first(g, i); !neigh_done(g); neigh = neigh_next(g)) {
            edge[k].from = i;
            edge[k].to = neigh;
            PriorityQueueAddItem(pq, rand(), edge+k);
            k++;
        }
    }
    while (PriorityQueueSize(pq)) {
        Edge *temp = PriorityQueueRemoveMinItem(pq);
        if (!mark[temp->from] && !mark[temp->to]) {
            mark[temp->from] = 1;
            mark[temp->to] = 1;
            vcSize+=2;
        }
    }
    free(edge);
    PriorityQueueDestroy(pq);
    return vcSize;
}
/***********************************************************************************************/
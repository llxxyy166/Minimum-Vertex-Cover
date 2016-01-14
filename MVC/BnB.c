//
//  BnB.c
//  MVC
//
//  Created by xinye lei on 15/12/4.
//  Copyright © 2015年 xinye lei. All rights reserved.
//

#include "BnB.h"
#include "PrioQ.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "FIFOQueue.h"

typedef struct MAX_DEGREE_GREEDY {
    unsigned node;
    unsigned degree;
}MDG;

MDG* sort(MDG *array, int size) {        //provide a list sorted by degree
    PQueue pq = PriorityQueueCreate();
    MDG *sortedArray = malloc(sizeof(MDG) * size);
    for (int i = 1; i < size; i++) {
        PriorityQueueAddItem(pq, array[i].degree, array+i);
    }
    for (int i = 1; i < size; i++) {
        MDG *temp = PriorityQueueRemoveMinItem(pq);
        sortedArray[size-i].degree = temp->degree;
        sortedArray[size-i].node = temp->node;
    }
    PriorityQueueDestroy(pq);
    return sortedArray;
}



//Configuration for the Branch and Bound algorithm
typedef struct Config {
    int vSize;    // number of vertices
    int eSize;    // number of edges
    MDG *vList;   // array hold vertices sorted by degree
    int *mark;    // mark whether a vertex is free
    //    int *degree;  // hold each vertex's current degree
    int sol;      // number of vertex selected for cover
    int best;     // best size of vertex cover
    int edgeToCover; // number of edges left need to be covered
    int *select;     // current vertex selection
    int *bestSelect; // vertex selection for current best solution
    clock_t startTime;
}Config;


Config* InitConfig(Graph *g, char *name) { //initialize the configuration
    int vSize = (int)getNumNodes(g);
    int eSize = (int)getEdge(g);
    Config *config = malloc(sizeof(Config));
    config->vSize = vSize;
    config->eSize = eSize;
    config->mark = malloc(sizeof(int) * (vSize + 1));
    config->select = malloc(sizeof(int) * (vSize + 1));
    config->bestSelect = malloc(sizeof(int) * (vSize + 1));
    for (int i = 1; i < vSize + 1; i++) {
        config->mark[i] = 0;
        config->select[i] = 0;
        config->bestSelect[i] = 0;
    }
    config->vList = malloc(sizeof(MDG) * (vSize+1));
    for (int i = 1; i < vSize + 1; i++) {
        config->vList[i].node = i;
        config->vList[i].degree = getDegree(g, i);
    }
    config->sol = 0;
    config->best = vSize;
    config->edgeToCover = eSize;
    const char *suffix = "_BnB_3600.sol";
    const char *suffix1 = "_BnB_3600.trace";
    char solName[50] = "";
    strcat(solName, name);
    strcat(solName, suffix);
    char traceName[50] = "";
    strcat(traceName, name);
    strcat(traceName, suffix1);

    
    config->startTime = clock();
    
    return config;
}

void Output(Config *config, file *f){
    fprintf(f->sol, "%d\n",config->best);
    for (int i = 1; i < config->vSize + 1; i++) {
        if (config->bestSelect[i]) {
            fprintf(f->sol, "%d,",i);
        }
    }
    fclose(f->sol);
}

int FreeConfig(Config *config) {    //free the configuration
    free(config->bestSelect);
    free(config->mark);
    free(config->select);
    free(config->vList);
    free(config);
    return 0;
}

Config* CopyConfig(Config* config) {  //get a copy of the configuration
    Config *copy = malloc(sizeof(Config));
    int vSize = config->vSize;
    copy->edgeToCover = config->edgeToCover;
    copy->eSize =config->eSize;
    copy->sol = config->sol;
    copy->vSize = config->vSize;
    copy->startTime = config->startTime;
    copy->mark = malloc(sizeof(int) * (vSize + 1));
    copy->select = malloc(sizeof(int) * (vSize + 1));
    copy->bestSelect = malloc(sizeof(int) * (vSize + 1));
    copy->vList = malloc(sizeof(MDG) * (vSize+1));
    for (int i = 1; i < vSize + 1; i++) {
        copy->vList[i].node = config->vList[i].node;
        copy->vList[i].degree = config->vList[i].degree;
        copy->mark[i] = config->mark[i];
        copy->select[i] = config->select[i];
    }
    return copy;
}

int RestoreConfig(Config *copy, Config *config) { //restore the configuration to the copy
    int vSize = config->vSize;
    copy->edgeToCover = config->edgeToCover;
    copy->eSize =config->eSize;
    copy->sol = config->sol;
    copy->vSize = config->vSize;
    copy->startTime = config->startTime;
    for (int i = 1; i < vSize + 1; i++) {
        copy->vList[i].node = config->vList[i].node;
        copy->vList[i].degree = config->vList[i].degree;
        copy->mark[i] = config->mark[i];
        copy->select[i] = config->select[i];
    }
    FreeConfig(config);
    return 0;
}


int B_and_B(Graph *g, double cutOff, Config *config, file *f) {
    clock_t now = clock(); //timer
    if ((now - config->startTime) / (double)CLOCKS_PER_SEC > cutOff) {
        printf("Time up\n");
        Output(config,f);
        FreeConfig(config);
        freeGraph(g);
        fclose(f->sol);
        fclose(f->trace);
        free(f);
        exit(1);
    }
    int vSize = config->vSize;
    if (config->edgeToCover == 0) { //solution found
        if (config->sol < config->best) { //compare to best solution
            config->best = config->sol;
            fprintf(f->trace, "%f,%d\n",(now - config->startTime) / (double) CLOCKS_PER_SEC, config->best);
            for (int i = 1; i < vSize + 1; i++) {
                config->bestSelect[i] = config->select[i]; //update best
            }
            printf("%d\n",config->best);
        }
        return 0;
    }
    MDG *newDegree = sort(config->vList, vSize + 1); //list sorted by degree
    int i,index = 0;                 //index is the vertex being proccesing (with maximum degree)
    int isLast = 1;                    //flag that indicate whether there are more vertices to go
    for (i = 1; i < vSize + 1; i++) { //get the first unmarked vertex (maximum degree)
        index = newDegree[i].node;
        //        index = config->vList[i].node;
        if (!config->mark[index]) {
            isLast = 0;
            break;
        }
    }
    if (isLast) {       //return when no more vertex to process
        free(newDegree);
        return 0;
    }
    //bound calculation. Since k vertices can cover at most d(1)+...d(k) edges
    // and the vertices left are sorted by degree, then the sum of first k degrees
    //is the bound. We can cover at most this number of edges. If the edge left
    //greater than it, we can cut this branch.
    
    int vToGo = config->best - config->sol - 1; //we can have at most this number vertices to choose as VC
    //-1 because we want a better one than our current best
    int M =0;     //bound
    for (int j = 1; j < vSize + 1; j++) {
        int pos = newDegree[j].node;
        if (!vToGo) {
            break;
        }
        if (!config->mark[pos]) {
            M += newDegree[j].degree;
            vToGo--;
        }
    }
    free(newDegree);
    if (M < config->edgeToCover) {   //cut branch
        return 0;
    }
    
    //go to left branch by select this vertex
    config->mark[index] = 1;
    config->edgeToCover -= config->vList[index].degree;
    config->sol++;
    config->select[index] = 1;
    for (unsigned neigh = neigh_first(g, index); !neigh_done(g); neigh = neigh_next(g)) {
        if (!config->select[neigh]) {
            config->vList[neigh].degree--;
        }
    }
    B_and_B(g, cutOff, config, f);
    
    
    //go to right branch by not select this vertex, but select all its neighbors
    config->edgeToCover += config->vList[index].degree;
    config->sol--;
    config->select[index] = 0;
    
    FIFOQueue *queue1 = fifo_create();
    FIFOQueue *queue2 = fifo_create();
    
    int goRight = 1; //if one of this vertex's neighbor is processed and not selected, then we
    //do not need to go to rightbranch because there will be an edge not covered.
    
    for (unsigned neigh = neigh_first(g, index); !neigh_done(g); neigh = neigh_next(g)) {
        if (!config->select[neigh]) {
            config->vList[neigh].degree++;
        }
        if (config->mark[neigh] && !config->select[neigh]) {
            goRight = 0;
        }
        if (!config->mark[neigh]) {
            fifo_push(queue1, &config->vList[neigh]);
        }
    }
    Config *copy = CopyConfig(config); //lots configuration will be changed so get a copy
    while (fifo_size(queue1)) {
        MDG *temp = fifo_pop(queue1);
        for (unsigned neigh = neigh_first(g, temp->node); !neigh_done(g); neigh = neigh_next(g)) {
            if (!config->select[temp->node]) {
                config->vList[neigh].degree--;
            }
        }
        fifo_push(queue2, temp);
    }
    while (fifo_size(queue2)) {
        MDG *temp = fifo_pop(queue2);
        int node = temp->node;
        config->mark[node] = 1;
        config->select[node] = 1;
        config->sol++;
        config->edgeToCover -= config->vList[node].degree;
    }
    fifo_free(queue2);
    fifo_free(queue1);
    if (goRight) {
        B_and_B(g, cutOff, config, f);
    }
    
    RestoreConfig(config, copy); //recover the configuration
    config->mark[index] = 0;     //unmark this vertex
    return 0;
}
/***********************************************************************************************/
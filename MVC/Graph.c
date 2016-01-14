//
//  Graph.c
//  Assignment 3
//
//  Created by Karl Gemayel on 10/26/15.
//  Copyright © 2015 Karl Gemayel. All rights reserved.
//

#include "Graph.h"
#include <stdlib.h>
#include <string.h>
#define MAXIMUM 999999
#define BUFFER_SIZE 1000000

// adjacency list element
typedef struct ALElement {
    
    unsigned from;
    unsigned to;
    int degree;
    struct ALElement *next;
    
} ALElement;


typedef struct Graph {
    
    size_t numNodes;
    
    size_t numEdges;
    
    ALElement** adjList;     // adjacency list
    
    ALElement* iter;        // used by iterator
    
    int *degree;
    
} Graph;





Graph* makeGraph(size_t numNodes) {
    Graph *g = (Graph*) malloc(sizeof(Graph));
    
    g->numNodes = numNodes;
    
    g->numEdges = 0;
    
    g->adjList = (ALElement **) malloc(sizeof(ALElement*) * (numNodes + 1));
    
    g->degree = malloc(sizeof(int) * (numNodes + 1));
    for (size_t i = 1; i < numNodes + 1; i++) {
        g->adjList[i] = NULL;
        g->degree[i] = 0;
    }
    
    g->iter = NULL;
    
    return g;
}

void freeGraph(Graph *g) {
    for (size_t n = 1; n < g->numNodes + 1; n++) {
        ALElement* p = g->adjList[n];
        
        while (p != NULL) {
            ALElement* temp = p;
            p = p->next;
            free(temp);
        }
            
        
    }
    
    free(g->adjList);
    free(g->degree);
    free(g);
}

void addEdge(Graph *g, unsigned from, unsigned to) {  //***//
    
    ALElement* e = malloc(sizeof(ALElement));
    e->from = from;
    e->to = to;
    e->next = NULL;
    g->degree[from]++;
    
    
    if (g->adjList[from] == NULL) {
        g->adjList[from] = e;
    }
    
    else {
        ALElement *p = g->adjList[from];
        
        // add to front of list
        e->next = p;
        g->adjList[from] = e;
    }
}


// iterator methods
unsigned neigh_first (Graph *g, unsigned from) {
    
    if (from > g->numNodes)
        return INT_FAST32_MAX;
    
    g->iter = NULL;                 // reset iterator
    
    if (g->adjList[from] == NULL)
        return INT_FAST32_MAX;
    
    g->iter = g->adjList[from];
    
    return g->iter->to;
    
}

unsigned neigh_next (Graph *g) {
    
    if (g->iter == NULL)
        return INT_FAST32_MAX;
    
    g->iter = g->iter->next;        // next element
    
    if (g->iter == NULL)
        return INT_FAST32_MAX;
    
    return g->iter->to;
    
}


int neigh_done (Graph *g) {
    return (g->iter == NULL);
}



size_t getNumNodes(Graph *g) {
    return g->numNodes;
}

unsigned getDegree(Graph *g, unsigned from){
    
    return g->degree[from];
    
}

void setEdge(Graph *g, size_t edge) {
    g->numEdges = edge;
}

size_t getEdge(Graph *g) {
    return g->numEdges;
}

Graph* readGraph(const char* filename) {
    
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Graph does not exist\n");
        return NULL;            // file doesn't exist
    }
    
    unsigned numNodes = 0;
    unsigned numEdges = 0;
    char buffer[BUFFER_SIZE];
    char *split;
    fgets(buffer, BUFFER_SIZE, fp);
    split = strtok(buffer, " ");
    numNodes = atoi(split);
    split = strtok(NULL, " ");
    numEdges = atoi(split);
    
    Graph* g = makeGraph(numNodes);     // create graph
    setEdge(g, numEdges);
    
    unsigned v;
    for (int i = 1; i < numNodes + 1; i++) {
        fgets(buffer, BUFFER_SIZE, fp);
        split = strtok(buffer, " ");
        while (split) {
            v = atoi(split);
            split = strtok(NULL, " ");
            if (v) {
                addEdge(g, i, v);
            }
        }
    }
    fclose(fp);
    return g;
}


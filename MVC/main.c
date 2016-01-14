//
//  main.c
//  MVC
//
//  Created by xinye lei on 15/11/9.
//  Copyright © 2015年 xinye lei. All rights reserved.
//
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Approx.h"
#include "BnB.h"


int main(int argc, const char * argv[]) {
    const char *fileName = argv[1];
    const char *method = argv[2];
    double cutOff = atof(argv[3]);
    unsigned seed = atoi(argv[4]);
    Graph *g = readGraph(fileName);
    size_t nodeSize = getNumNodes(g);
    srand(seed);
    if (argc != 5) {
        printf("see README for usage\n");
        return 1;
    }
    if (strncmp(method, "BnB", 3) && strncmp(method, "Approx", 6)) {
        printf("abc\n");
        return 1;
    }

    
    const char *name = strtok((char*)fileName,".");
    char solName[50] = "";
    strcat(solName, name);
    strcat(solName, "_");
    strcat(solName, method);
    strcat(solName, "_");
    strcat(solName, argv[3]);
    strcat(solName, ".sol");
    char traceName[50] = "";
    strcat(traceName, name);
    strcat(traceName, "_");
    strcat(traceName, method);
    strcat(traceName, "_");
    strcat(traceName, argv[3]);
    strcat(traceName, ".trace");
    
    file *f = malloc(sizeof(file));
    if (!strncmp(method, "BnB", 1)) {
        FILE *sol = fopen(solName, "w");
        FILE *trace = fopen(traceName, "w");
        f->sol = sol;
        f->trace = trace;
        Config *config = InitConfig(g, (char*)name);
        B_and_B(g, cutOff, config, f);
        Output(config, f);
        fclose(f->trace);
        FreeConfig(config);
    }
    
    if (!strncmp(method, "Approx", 6)) {
        FILE *sol = fopen(solName, "w");
        f->sol = sol;
        MDG_SOLUTION(g, f->sol);
    }

    fclose(f->sol);
    free(f);
    freeGraph(g);
 



}

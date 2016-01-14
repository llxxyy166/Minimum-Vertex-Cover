//
//  BnB.h
//  MVC
//
//  Created by xinye lei on 15/12/4.
//  Copyright © 2015年 xinye lei. All rights reserved.
//

#ifndef BnB_h
#define BnB_h

#include <stdio.h>
#include "Graph.h"

typedef struct Config Config;
typedef struct file {
    FILE *sol;
    FILE *trace;
}file;

Config* InitConfig(Graph *g, char *name);
int FreeConfig(Config *config);
void Output(Config *config, file *f);

int B_and_B(Graph *g, double cutOff, Config *config, file *f);

#endif /* BnB_h */

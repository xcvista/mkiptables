//
//  global.c
//  mkiptables
//
//  Created by Maxthon Chan on 1/30/14.
//  Copyright (c) 2014 Maxthon Chan. All rights reserved.
//

#include "global.h"

vector_t extif;
vector_t intif;
vector_t mapping;
int logging;

void __attribute__((constructor)) init_global(void)
{
    extif = vector_init_empty(sizeof(ipaddr_t), 10);
    intif = vector_init_empty(sizeof(ipaddr_t), 10);
    mapping = vector_init_empty(sizeof(mapping_t), 20);
    logging = 0;
}

void __attribute__((destructor)) fini_global(void)
{
    vector_fini(extif);
    extif = NULL;
    vector_fini(intif);
    intif = NULL;
    vector_fini(mapping);
    mapping = NULL;
}

//
//  global.h
//  mkiptables
//
//  Created by Maxthon Chan on 1/30/14.
//  Copyright (c) 2014 Maxthon Chan. All rights reserved.
//

#ifndef mkiptables_global_h
#define mkiptables_global_h

#include <sys/cdefs.h>
#include "vector.h"

__BEGIN_DECLS

typedef struct ipaddr_s
{
    char addr[20];
} ipaddr_t;

typedef struct mapping_s
{
    char protocol[10];
    ipaddr_t source;
    int sport;
    ipaddr_t dest;
    int dport;
} mapping_t;

extern vector_t extif;
extern vector_t intif;
extern vector_t mapping;
extern int logging;

__END_DECLS

#endif

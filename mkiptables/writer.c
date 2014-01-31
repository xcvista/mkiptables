//
//  writer.c
//  mkiptables
//
//  Created by Maxthon Chan on 1/31/14.
//  Copyright (c) 2014 Maxthon Chan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "global.h"

void emit_iptables(void)
{
    // Validate configuration
    
    // At least one extif
    if (vector_count(extif) < 1)
    {
        fprintf(stderr, "error: no external interface defined.\n");
        exit(EXIT_FAILURE);
    }
    
    // At least one intif
    if (vector_count(intif) < 1)
    {
        fprintf(stderr, "error: no internal interface defined.\n");
        exit(EXIT_FAILURE);
    }
    
    // TODO: Add mapping validation. Currently it is up to iptables to validate
    // all mappings.
    
    // Print outputs.
    
    // NAT table
    fprintf(stdout,
            "*nat\n");
    
    for (size_t idx = 0; idx < vector_count(extif); idx++)
    {
        ipaddr_t interface;
        vector_objectat(extif, &interface, idx);
        
        fprintf(stdout,
                "-A POSTROUTING -o %s -j MASQUERADE\n", interface.addr);
        
        for (size_t idx = 0; idx < vector_count(mapping); idx++)
        {
            mapping_t map;
            vector_objectat(mapping, &map, idx);
            
            if (map.sport > 65535 || map.sport <= 0)
            {
                map.sport = -1;
            }
            
            if (map.dport > 65535 || map.dport <= 0)
            {
                map.dport = -1;
            }
            
            
            //if (!(strcmp(map.source.addr, "0") && strcmp(map.source.addr, "0.0.0.0") && strcmp(map.source.addr, "any")))
            //    ; // Not used for now.
            
            fprintf(stdout, "-A PREROUTING -p %s ", map.protocol);
            
            if (map.sport > 0)
                fprintf(stdout, "--dport %d ", map.sport);
            
            fprintf(stdout, "-i %s -j DNAT --to %s", interface.addr, map.dest.addr);
            
            if (map.dport > 0)
                fprintf(stdout, ":%d", map.dport);
            
            fprintf(stdout, "\n");
        }
    }
    
    // TODO: Not using a transparent (forward-all) if possible.
    
    fprintf(stdout,
            "COMMIT\n"
            "*filter\n"
            ":INPUT ACCEPT [0:0]\n"
            ":FORWARD ACCEPT [0:0]\n"
            ":OUTPUT ACCEPT [0:0]\n"
            );
    
    if (logging)
    {
        fprintf(stdout, "-A FORWARD -j LOG\n");
    }
    
    fprintf(stdout, "COMMIT\n");
    
    fflush(stdout);
}
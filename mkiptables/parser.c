//
//  parser.c
//  mkiptables
//
//  Created by Maxthon Chan on 1/30/14.
//  Copyright (c) 2014 Maxthon Chan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "parser.h"
#include "global.h"

#pragma mark - Token state machine

typedef enum token_state_e {
    s_outside,
    s_extif,
    s_intif,
    s_map_proto,
    s_map_source,
    s_map_dest,
    s_logging,
} token_state_t;

token_state_t current;
mapping_t current_mapping;

// NOTE: Return with *port = -1 means missing or any port.
int handle_addr_port(const char *token, ipaddr_t *addr, int *port)
{
    char *lastColon = strrchr(token, ':');
    ipaddr_t add;
    memset(&add, 0, sizeof(ipaddr_t));
    
    if (lastColon)
    {
        strncpy(add.addr, token, lastColon - token);
        
        char *ep = NULL;
        int p = (int)strtol(++lastColon, &ep, 10);
        if (*ep)
            p = -1;
        
        if (addr)
            *addr = add;
        if (port)
            *port = p;
    }
    else
    {
        strncpy(add.addr, token, 20);
        if (addr)
            *addr = add;
        if (port)
            *port = -1;
    }
    return 0;
}

int handle_token(const char *token)
{
    switch (current)
    {
        case s_outside:
        {
            if (!strcmp(token, "ext"))
                current = s_extif;
            else if (!strcmp(token, "int"))
                current = s_intif;
            else if (!strcmp(token, "map"))
                current = s_map_proto;
            else if (!strcmp(token, "log"))
                current = s_logging;
            else
                return 1;
        }
            break;
        case s_extif:
        {
            ipaddr_t ext;
            memset(ext.addr, 0, 20);
            strcpy(ext.addr, token);
            vector_append(extif, &ext);
            current = s_outside;
        }
            break;
        case s_intif:
        {
            ipaddr_t in;
            memset(&in, 0, sizeof(ipaddr_t));
            strcpy(in.addr, token);
            vector_append(intif, &in);
            current = s_outside;
        }
            break;
        case s_map_proto:
        {
            memset(&current_mapping, 0, sizeof(mapping_t));
            strcpy(current_mapping.protocol, token);
            current = s_map_source;
        }
            break;
        case s_map_source:
        {
            ipaddr_t addr;
            int port;
            int rv = handle_addr_port(token, &addr, &port);
            if (!rv)
            {
                current_mapping.source = addr;
                current_mapping.sport = port;
                current = s_map_dest;
            }
            else
            {
                return rv;
            }
        }
            break;
        case s_map_dest:
        {
            ipaddr_t addr;
            int port;
            int rv = handle_addr_port(token, &addr, &port);
            if (!rv)
            {
                current_mapping.dest = addr;
                current_mapping.dport = port;
                vector_append(mapping, &current_mapping);
                current = s_outside;
            }
            else
            {
                return rv;
            }
        }
            break;
        case s_logging:
        {
            if (!strcmp(token, "on"))
                logging = 1;
            else if (!strcmp(token, "off"))
                logging = 0;
            else
                return 1;
            current = s_outside;
        }
            break;
        default:
        {
            fprintf(stderr, "weird: unexpected state: %d\n", current);
            exit(-1);
        }
            break;
    }
    return 0;
}

#pragma mark - Parser

void chop(char *str, int ch)
{
    char *comment = strchr(str, ch);
    if (comment)
        *comment = 0;
}

void parse_input(FILE *file)
{
    int linenum = 0;
    char *linebuf = malloc(BUFSIZ);
    
    if (!linebuf)
    {
        fprintf(stderr, "weird: cannot allocate memory: %s.\n", strerror(errno));
        exit(-1);
    }
    
    while (fgets(linebuf, BUFSIZ, file))
    {
        ++linenum;
        
        char *chidx = linebuf;
        int step;
        
        chop(linebuf, '\n');
        chop(linebuf, '#');
        
        char command[30];
        memset(command, 0, 20);
        
        while (sscanf(chidx, "%29s%n", command, &step) != EOF)
        {
            switch (handle_token(command))
            {
                case 0: // ok
                    chidx += step;
                    continue;
                case 1:
                    fprintf(stderr, "error: unexpected token in line %d row %d: %s\n", linenum, (int)(chidx - linebuf), command);
                    exit(1);
                default:
                    fprintf(stderr, "weird: handle_token returned a weird value.\n");
                    exit(-1);
                    break;
            }
        }
    }
}

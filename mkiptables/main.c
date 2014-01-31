//
//  main.c
//  mkiptables
//
//  Created by Maxthon Chan on 1/29/14.
//  Copyright (c) 2014 Maxthon Chan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "help.h"
#include "options.h"
#include "parser.h"
#include "writer.h"

int main(int argc, const char * argv[])
{
    FILE *inFile = NULL;
    int opt = 0;
    
    opterr = 0;
    optind = 0;
    optopt = 0;
    
    while ((opt = getopt_long(argc,
                              (char **)argv,
                              options,
                              options_long,
                              NULL)) != -1)
    {
        switch (opt)
        {
            case 'h':
                help();
                exit(EXIT_SUCCESS);
                break;
            case 'v':
                version();
                exit(EXIT_SUCCESS);
            case '?':
                if (isprint(*optarg))
                {
                    fprintf(stderr, "error: unrecognized argument: %s\n", optarg);
                }
                else
                {
                    fprintf(stderr, "error: unrecognized character: \\x%x\n", *optarg);
                }
            default:
                help();
                exit(EXIT_FAILURE);
                break;
        }
    }
    
    argc -= optind;
    argv += optind;
    
    if (argc > 0)
    {
        if (!strcmp(argv[0], "-"))
        {
            inFile = stdin;
        }
        else if (argv[0])
        {
            FILE *f = fopen(argv[0], "r");
            if (!f)
            {
                fprintf(stderr, "error: cannot open %s: %s.\n", argv[0], strerror(errno));
                exit(EXIT_FAILURE);
            }
            else
            {
                inFile = f;
            }
        }
    }
    else
    {
        FILE *f = fopen(DEFAULT_CONF, "r");
        if (!f)
        {
            fprintf(stderr, "error: cannot open %s: %s.\n", DEFAULT_CONF, strerror(errno));
            exit(EXIT_FAILURE);
        }
        else
        {
            inFile = f;
        }
    }
    
    if (!inFile)
    {
        fprintf(stderr, "weird: I don't have an input file.");
        exit(-1);
    }
    
    parse_input(inFile);
    
    emit_iptables();
}


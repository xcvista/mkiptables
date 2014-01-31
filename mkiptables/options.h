//
//  options.h
//  mkiptables
//
//  Created by Maxthon Chan on 1/29/14.
//  Copyright (c) 2014 Maxthon Chan. All rights reserved.
//

#ifndef mkiptables_options_h
#define mkiptables_options_h

#include <getopt.h>

const static struct option options_long[] = {
    { "help",    no_argument, NULL, 'h'},
    { "version", no_argument, NULL, 'v'},
    { NULL,      0,           NULL, 0  }
};

const static char *options = "hv";

#define DEFAULT_CONF "/etc/portmap.conf"

#endif

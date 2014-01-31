//
//  help.c
//  mkiptables
//
//  Created by Maxthon Chan on 1/30/14.
//  Copyright (c) 2014 Maxthon Chan. All rights reserved.
//

#include <stdio.h>

#include "help.h"

#if __has_include("version.h")
#include "version.h"
#endif

void title(void)
{
#ifdef GIT_VERSION
    fprintf(stderr, "mkiptables 2.0 (git %s) - Linux iptables generator for NAT routers\n", GIT_VERSION);
#else
    fprintf(stderr, "mkiptables 2.0 - Linux iptables generator for NAT routers\n");
#endif
}

void copyright(void)
{
    fprintf(stderr,
            "Copyright (c) 2014 Maxthon Chan. All rights reserved.\n"
            "This is free software. You are permitted and encouraged to use, modify,\n"
            "copy and distribute this software.\n"
            "THIS SOFTWARE IS PROVIDED TO YOU BY AUTHORS ON AN \"AS-IS\" BASIS. NO\n"
            "WARRANTY WHATSOEVER IS PROVIDED WITH THIS SOFTWARE FROM ITS AUTHORS TO\n"
            "THE LIMITATION PERMITTED BY APPLICATABLE LAW. YOU USE THIS SOFTWARE AT\n"
            "YOUR OWN RISK. THE AUHTORS OF THIS SOFTWARE IS NOT RESPONSIBLE TO ANY\n"
            "LOSS YOU MAY EXPERIENCE FROM USING OF THIS SOFTWARE, INCLUDING BUT NOT\n"
            "LIMITED TO LOSS OF DATA, DISRUPTION OF BUSINESS AND LOSS OF FINANCIAL\n"
            "INTEREST. DO NOT USE THIS SOFTWARE ON CRITICAL SYSTEMS.\n");
}

void help(void)
{
    title();
    fprintf(stderr,
            "\n"
            "Usage: mkiptables [<options>] [<portmap.conf>|-]\n"
            "\n"
            "Open and read the file given, standard input (-) or /etc/portmap.conf\n"
            "and generate iptables-restore script accordingly.\n"
            "\n"
            "Options:\n"
            "    -h | --help:    Display this help message.\n"
            "    -v | --version: Display version information.\n"
            "\n"
            "Typical usage: mkiptables /etc/portmap.conf | iptables-restore\n"
            "\n");
    copyright();
}

void version(void)
{
    title();
    copyright();
}
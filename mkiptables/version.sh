#!/bin/bash

cat << EOF
/* version.h */

#ifndef _VERSION_H_
#define _VERSION_H_

EOF

if [ -e .git -o -e ../.git ]; then
    echo "#define GIT_VERSION $(git rev-parse --short HEAD)"
fi

cat << EOF
#define VERSION 2.0

#endif
EOF

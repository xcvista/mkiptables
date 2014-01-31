#!/bin/bash

if [ -x "$(which clang)" ]; then
    echo "clang"
elif [ -x "$(which gcc)" ]; then
    echo "gcc"
elif [ -x "$(which cc)" ]; then
    echo "cc"
else
    exit 1
fi

#!/bin/bash

# Set the output binary name
OUTPUT="main"

# Set the source files
SOURCES="main.c text_highlighter.c text_highlighter.h"

# Set the common flags
CFLAGS="-Wall -Wextra"

# Check for -debug flag
if [[ "$1" == "-debug" ]]; then
    echo "Compiling with debug symbols..."
    gcc $CFLAGS -g $SOURCES -o $OUTPUT
    if [ $? -eq 0 ]; then
        echo "Compilation successful!"
        gdb ./$OUTPUT $2
    else
        echo "Compilation failed!"
        exit 1
    fi
else
    echo "Compiling normally..."
    gcc $CFLAGS $SOURCES -o $OUTPUT
    if [ $? -eq 0 ]; then
        echo "Compilation successful!"
        ./$OUTPUT $1
    else
        echo "Compilation failed!"
        exit 1
    fi
fi

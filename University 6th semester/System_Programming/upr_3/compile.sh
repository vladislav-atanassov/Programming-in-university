#!/bin/bash

# Check for -valgrind flag
if [[ "$1" == "-valgrind" ]]; then
    echo "Compiling with AddressSanitizer..."
    gcc -Wall -Wextra -g -fsanitize=address main.c -o main
    if [ $? -eq 0 ]; then
        echo "Compilation successful!"
    else
        echo "Compilation failed!"
        exit 1
    fi
    ./main
else
    echo "Compiling normally..."
    gcc -Wall -Wextra main.c -o main
    if [ $? -eq 0 ]; then
        echo "Compilation successful!"
    else
        echo "Compilation failed!"
        exit 1
    fi
    ./main
fi

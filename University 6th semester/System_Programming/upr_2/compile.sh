#!/bin/bash

# Check for -valgrind flag
if [[ "$1" == "-valgrind" ]]; then
    echo "Compiling with AddressSanitizer..."
    gcc -Wall -Wextra -g -fsanitize=address utf8.c main.c read_convert_file.c grammar_correction.c colouring.c dictionary.c -o main
else
    echo "Compiling normally..."
    gcc -Wall -Wextra utf8.c main.c read_convert_file.c grammar_correction.c colouring.c dictionary.c -o main
fi

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
else
    echo "Compilation failed!"
    exit 1
fi

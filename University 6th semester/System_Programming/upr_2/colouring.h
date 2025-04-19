#ifndef COLOURING_H
#define COLOURING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET_ANSI          "\033[0m\0"
#define REMOVE_HIGHLIGHT    "\033[47;31m\0"
#define ADD_HIGHLIGHT       "\033[44;33m\0"

int is_marked_for_correction(int *corrections, int index);
void highlight_to_file(int *corrections, char *chunk, FILE *output_file, int type);

#endif // COLOURING_H
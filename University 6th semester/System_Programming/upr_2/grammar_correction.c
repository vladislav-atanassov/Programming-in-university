#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar_correction.h"

int is_symbol(char ch, const char *symbols)
{
    return strchr(symbols, ch) != NULL;
}

void remove_mult_spaces(char *chunk)
{
    int i, j = 0;
    int in_symbol = 0;

    for (i = 0; chunk[i] != '\0'; i++)
    {
        if (chunk[i] == ' ')
        {
            if (!in_symbol)
            {
                chunk[j++] = chunk[i];
                in_symbol = 1;
            }
        }
        else
        {
            chunk[j++] = chunk[i];
            in_symbol = 0;
        }
    }
    chunk[j] = '\0';
}

void remove_space_before_symbols(char *chunk)
{
    int i, j = 0;

    for (i = 0; chunk[i] != '\0'; i++)
    {
        if ((chunk[i] == ' ') && is_symbol(chunk[i + 1], NO_SPACE_BEFORE_SYMBOLS))
        {
            continue;
        }
        chunk[j++] = chunk[i];
    }
    chunk[j] = '\0';
}

void adjust_dots(char *chunk)
{
    int write_index = 0;
    int dot_count = 0;

    for (int i = 0; chunk[i] != '\0'; i++)
    {
        if (chunk[i] == '.')
        {
            dot_count++;
            if (dot_count <= 3)
            {
                chunk[write_index++] = '.';
            }
        }
        else
        {
            if (dot_count == 2)
            {
                write_index--; // Remove second dot
            }
            dot_count = 0;
            chunk[write_index++] = chunk[i];
        }
    }

    if (dot_count == 2)
    {
        write_index--; // Remove second dot if at end
    }
    chunk[write_index] = '\0';
}

void add_space_after_comma(char *chunk)
{
    int len = strlen(chunk);
    for (int i = 0; i < len - 1; i++)
    {
        if ((chunk[i] == ',') && (chunk[i + 1] != ' '))
        {
            memmove(&chunk[i + 2], &chunk[i + 1], len - i);
            chunk[i + 1] = ' ';
            len++;
        }
    }
}

void remove_comma_from_end_of_sentance(char *chunk)
{
    int len = strlen(chunk);
    for (int i = 0; i < len; i++)
    {
        if ((chunk[i] == ',') &&
            ((is_symbol(chunk[i + 1], END_OF_SENTENCE_SYMBOLS)) ||
             (is_symbol(chunk[i - 1], END_OF_SENTENCE_SYMBOLS))))
        {
            memmove(&chunk[i], &chunk[i + 1], len - i);
            len--;
            i--; // Adjust index to check next character
        }
    }
}

void fix_direct_speech_error(char *chunk)
{
    int len = strlen(chunk);
    for (int i = 0; i < len; i++)
    {
        if (chunk[i] == '\n')
        {
            memmove(&chunk[i], &chunk[i + 1], len - i);
            len--;
            i--; // Adjust index to check next character
        }
        if (chunk[i] == '-')
        {
            // Ensure we don't access out of bounds
            if (((i + 1) < len) && (chunk[i + 1] != ' '))
            {
                memmove(&chunk[i + 2], &chunk[i + 1], len - i);
                chunk[i + 1] = ' ';
                len++;
                chunk[len] = '\0'; // Ensure null termination
            }
            if ((i > 0) && (chunk[i - 1] != ' '))
            {
                memmove(&chunk + i - 1, &chunk[i - 2], len - i);
                chunk[i - 1] = ' ';
                len++;
                chunk[len] = '\0'; // Ensure null termination
            }
        }
    }
}

void grammar_correction(char *chunk)
{
    remove_mult_spaces(chunk);
    remove_space_before_symbols(chunk);
    adjust_dots(chunk);
    remove_comma_from_end_of_sentance(chunk);
    add_space_after_comma(chunk);
    fix_direct_speech_error(chunk);
}

#include <wchar.h>
#include <locale.h> 

#include "utf8.h"
#include "grammar_correction.h"
#include "colouring.h"

int is_marked_for_correction(int *corrections, int index)
{
    for (size_t i = 0; corrections[i] != -1; i++)
    {
        if (corrections[i] == index)
        {
            return 1;
        }
    }
    return 0;
}

void highlight_to_file(int *corrections, char *chunk, FILE *output_file, int type)
{
    setlocale(LC_CTYPE, ""); // Enable UTF-8 locale support

    size_t i = 0;
    size_t len = strlen(chunk);
    const char *highlight_type = (type == TYPE_REMOVE) ? REMOVE_HIGHLIGHT : ADD_HIGHLIGHT;

    while (i < len)
    {
        size_t char_len = utf8_num_bytes(chunk + i); // Get length of multi-byte char

        if (is_marked_for_correction(corrections, i))
        {
            fwrite(highlight_type, 1, strlen(highlight_type), output_file);
            fwrite(&chunk[i], 1, char_len, output_file); // Write highlighted char
            fwrite(RESET_ANSI, 1, strlen(RESET_ANSI), output_file);
        }
        else
        {
            fwrite(&chunk[i], 1, char_len, output_file); // Write normal char
        }

        i += char_len;
    }

    if (type == TYPE_ADD)
    {
        fwrite("\n", 1, 1, output_file);
    }
}

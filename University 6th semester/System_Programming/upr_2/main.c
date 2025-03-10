#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>  // open()
#include <unistd.h> // lseek(), close()
#include <fcntl.h>

#include "read_convert_file.h"
#include "grammar_correction.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <utf8_input_file>\n", argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *temp_filename = "temp_single_byte.txt";

    size_t buffer_size = 0;

    get_file_size(input_filename, &buffer_size);

    if (convert_file_encoding(input_filename, temp_filename, "utf-8", "WINDOWS-1251", buffer_size) != 0)
    {
        fprintf(stderr, "Conversion to single-byte encoding failed.\n");
        return 1;
    }

    printf("Processed output:\n");
    process_temp_file(temp_filename);
    printf("\n");

    return 0;
}

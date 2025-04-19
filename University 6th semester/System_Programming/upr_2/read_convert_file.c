#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "utf8.h"

#include "read_convert_file.h"
#include "grammar_correction.h"
#include "colouring.h"

void get_file_size(const char *input_file_name, size_t *buffer_size)
{
    const int file_d = open(input_file_name, O_RDONLY);
    if (file_d == -1)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    *buffer_size = lseek(file_d, 0, SEEK_END);
    if (*buffer_size == (size_t)-1)
    {
        perror("lseek failed");
        close(file_d); // Ensure file is closed
        exit(EXIT_FAILURE);
    }

    if (*buffer_size == 0)
    {
        fprintf(stderr, "File is empty\n");
    }

    close(file_d);
}

int convert_file_encoding(const char *input_filename, const char *output_filename,
                          const char *from_charset, const char *to_charset, size_t buffer_size)
{
    FILE *infile = fopen(input_filename, "rb");
    if (!infile)
    {
        perror("Error opening input file");
        return -1;
    }

    FILE *outfile = fopen(output_filename, "wb");
    if (!outfile)
    {
        perror("Error opening output file");
        fclose(infile);
        return -1;
    }

    iconv_t cd = iconv_open(to_charset, from_charset);
    if (cd == (iconv_t)-1)
    {
        perror("Error initializing iconv");
        fclose(infile);
        fclose(outfile);
        return -1;
    }

    char *inbuf = malloc(buffer_size);
    char *outbuf = malloc(buffer_size * 2);
    if ((NULL == inbuf) || (NULL == outbuf))
    {
        perror("Memory allocation failed");
        free(inbuf);
        free(outbuf);
        iconv_close(cd);
        fclose(infile);
        fclose(outfile);
        return -1;
    }

    size_t inbytes, outbytes;
    while ((inbytes = fread(inbuf, 1, buffer_size, infile)) > 0)
    {
        char *inptr = inbuf;
        char *outptr = outbuf;
        outbytes = buffer_size * 2;

        if (iconv(cd, &inptr, &inbytes, &outptr, &outbytes) == (size_t)-1)
        {
            perror("iconv conversion error");
            break;
        }

        if (fwrite(outbuf, 1, buffer_size * 2 - outbytes, outfile) == 0)
        {
            perror("Error writing to output file");
            break;
        }
    }

    free(inbuf);
    free(outbuf);
    iconv_close(cd);
    fclose(infile);
    fclose(outfile);
    return 0;
}

int convert_chunk(const char *input, size_t input_size, char *output, size_t output_size,
                  const char *from_charset, const char *to_charset)
{
    iconv_t cd = iconv_open(to_charset, from_charset);
    if (cd == (iconv_t)-1)
    {
        perror("iconv_open");
        return -1;
    }

    char *inptr = (char *)input;
    char *outptr = output;
    size_t inbytes = input_size;
    size_t outbytes = output_size;

    if (iconv(cd, &inptr, &inbytes, &outptr, &outbytes) == (size_t)-1)
    {
        perror("iconv");
        iconv_close(cd);
        return -1;
    }

    *outptr = '\0';
    iconv_close(cd);
    return 0;
}

void process_temp_file(const char *temp_filename, const char *output_filename)
{
    FILE *temp = fopen(temp_filename, "rb");
    if (NULL == temp)
    {
        perror("temp: Error opening temporary file");
        return;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (NULL == output_file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    size_t chunk_size = INITIAL_CHUNK_SIZE;
    char *buffer = calloc(chunk_size, sizeof(char));

    if (NULL == buffer)
    {
        perror("Memory allocation error");

        free(buffer);
        buffer = NULL;
        fclose(temp);
        return;
    }

    size_t buffer_pos = 0;
    signed int ch;

    while ((ch = fgetc(temp)) != EOF)
    {
        if (buffer_pos >= chunk_size - 1)
        {
            size_t new_size = chunk_size * 2;
            char *new_buffer = realloc(buffer, (new_size * sizeof(char)));

            if (NULL == new_buffer)
            {
                perror("Realloc failed");

                free(buffer);
                buffer = NULL;
                fclose(temp);
                return;
            }

            buffer = new_buffer;
            chunk_size = new_size;
        }

        buffer[buffer_pos++] = (char)ch;

        // Skip the newline character if this is true for potential direct speech error
        if ((3 <= buffer_pos) &&
            ('\n' == buffer[buffer_pos - 1]) &&
            (' ' == buffer[buffer_pos - 2]) &&
            (',' == buffer[buffer_pos - 3]))
        {
            continue;
        }

        if ('\n' == ch)
        {
            buffer[buffer_pos] = '\0';

            char *utf8_chunk = malloc(chunk_size * 2);
            char *corrected_chunk = malloc(chunk_size * 2);
            utf8_chunk[chunk_size] = '\0';
            corrected_chunk[chunk_size] = '\0';

            if ((NULL == utf8_chunk) || (NULL == corrected_chunk))
            {
                perror("Memory allocation error");
                return;
            }

            if (convert_chunk(buffer, buffer_pos, utf8_chunk, chunk_size * 2,
                              "WINDOWS-1251", "UTF-8") != 0)
            {
                return;
            }

            // // Wait for the user to press ENTER to print the paragraph
            // while ('\n' != getchar())
            // {
            // }

            chunk_size = utf8_strlen(utf8_chunk);

            strcpy(corrected_chunk, utf8_chunk);

            printf("Original text from file:\t%s", corrected_chunk);
            grammar_correction(corrected_chunk, utf8_chunk, output_file, chunk_size);
            printf("Corrected text:\t\t\t%s\n", corrected_chunk);

            free(utf8_chunk);
            free(corrected_chunk);
            utf8_chunk = NULL;
            corrected_chunk = NULL;

            buffer_pos = 0;
        }
    }

    free(buffer);
    buffer = NULL;

    fclose(output_file);
    fclose(temp);
}
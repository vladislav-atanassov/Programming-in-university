#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>  // open()
#include <unistd.h> // lseek(), close()
#include <fcntl.h>

#include "read_convert_file.h"
#include "grammar_correction.h"

void get_file_size(const char *input_file_name, size_t *buffer_size)
{
    const int file_d = open(input_file_name, O_RDONLY);
    *buffer_size = lseek(file_d, 0, SEEK_END);

    if (file_d == -1)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    if (*buffer_size == (size_t)-1)
    {
        perror("lseek failed");
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

    char inbuf[buffer_size];
    char outbuf[buffer_size * 2];
    size_t inbytes, outbytes;

    while ((inbytes = fread(inbuf, 1, buffer_size, infile)) > 0)
    {
        char *inptr = inbuf;
        while (inbytes > 0)
        {
            char *outptr = outbuf;
            outbytes = sizeof(outbuf);
            size_t result = iconv(cd, &inptr, &inbytes, &outptr, &outbytes);
            if (result == (size_t)-1)
            {
                perror("iconv conversion error");
                iconv_close(cd);
                fclose(infile);
                fclose(outfile);
                return -1;
            }
            size_t converted = sizeof(outbuf) - outbytes;
            if (fwrite(outbuf, 1, converted, outfile) != converted)
            {
                perror("Error writing to output file");
                iconv_close(cd);
                fclose(infile);
                fclose(outfile);
                return -1;
            }
        }
    }

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

void process_temp_file(const char *temp_filename)
{
    FILE *temp = fopen(temp_filename, "rb");
    if (!temp)
    {
        perror("Error opening temporary file");
        return;
    }

    size_t chunk_size = INITIAL_CHUNK_SIZE;
    char *buffer = malloc(chunk_size);

    if (!buffer)
    {
        perror("Memory allocation error");
        fclose(temp);
        return;
    }

    size_t buffer_pos = 0;
    signed int ch;
    int flag_read_next_line = 0;

    while ((ch = fgetc(temp)) != EOF)
    {
        // Expand the buffer if needed.
        if (buffer_pos == chunk_size - 1)
        {
            chunk_size *= 2;
            char *new_buffer = realloc(buffer, chunk_size);
            if (!new_buffer)
            {
                perror("Memory allocation error");
                free(buffer);
                fclose(temp);
                return;
            }
            buffer = new_buffer;
        }

        buffer[buffer_pos++] = (char)ch;

        // If this is true that means there is incorrect direct speech syntax that should be in the same line
        if (buffer[buffer_pos - 1] == '\n' &&
           (buffer[buffer_pos - 2] == ' ' && buffer[buffer_pos - 3] == ',') ||
           (buffer[buffer_pos - 2] == ','))
        {
            continue;
        }

        // Process the chunk
        if (ch == '\n')
        {
            buffer[buffer_pos] = '\0';

            char utf8_chunk[chunk_size * 2];
            if (convert_chunk(buffer, buffer_pos, utf8_chunk, sizeof(utf8_chunk),
                              "WINDOWS-1251", "UTF-8") != 0)
            {
                fprintf(stderr, "Error converting chunk\n");
                buffer_pos = 0;

                continue;
            }

            grammar_correction(utf8_chunk);

            // Wait for the user to press ENTER (newline)
            while (getchar() != '\n')
            {
            }
            printf("%s", utf8_chunk);

            buffer_pos = 0;
        }
    }

    // Process if any remaining text.
    if (buffer_pos > 0)
    {
        buffer[buffer_pos] = '\0';
        char utf8_chunk[chunk_size * 2];
        if (convert_chunk(buffer, buffer_pos, utf8_chunk, sizeof(utf8_chunk),
                          "WINDOWS-1251", "UTF-8") == 0)
        {
            grammar_correction(utf8_chunk);
            printf("%s", utf8_chunk);
        }
    }

    free(buffer);
    fclose(temp);
}
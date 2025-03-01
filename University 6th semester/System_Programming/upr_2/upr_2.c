#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#define BUFFER_SIZE 4096
#define CHUNK_SIZE 50 // 50 bytes (characters) per chunk in the temporary file

const char no_space_before_symbols[] = "-,:;.!?";

int is_no_space_before_symbols(char ch, const char *symbols)
{
    for (int i = 0; i < 7; i++)
    {
        if (ch == symbols[i])
        {
            return 1;
        }
    }

    return 0;
}

void remove_mult_symbols(char *buffer, const char *symbols)
{
    int i, j = 0;
    int in_symbol = 0; // Flag to detect consecutive spaces

    for (i = 0; buffer[i] != '\0'; i++)
    {
        if (is_no_space_before_symbols(buffer[i], symbols))
        {
            if (!in_symbol)
            {
                buffer[j++] = buffer[i]; // Copy the first space
                in_symbol = 1;
            }
        }
        else
        {
            buffer[j++] = buffer[i]; // Copy non-space character
            in_symbol = 0;
        }
    }
    buffer[j] = '\0';
}

void remove_space_before_symbols(char *buffer, const char *symbols)
{
    int i, j = 0;

    for (i = 0; buffer[i] != '\0'; i++)
    {
        // If current char is space and next char is no_space_before_symbols, skip the space
        if (buffer[i] == ' ')
        {
            if (is_no_space_before_symbols(buffer[i + 1], symbols))
            {
                continue; // Do not copy this space
            }
        }

        buffer[j++] = buffer[i]; // Copy character to new position
    }
    buffer[j] = '\0';
}

// Step 1: Convert the entire file from one encoding to another.
int convert_file_encoding(const char *input_filename, const char *output_filename,
                          const char *from_charset, const char *to_charset)
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

    char inbuf[BUFFER_SIZE];
    char outbuf[BUFFER_SIZE * 2]; // Output buffer may need to be larger.
    size_t inbytes, outbytes;

    while ((inbytes = fread(inbuf, 1, BUFFER_SIZE, infile)) > 0)
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

// Utility function to convert a chunk of data using iconv.
int convert_chunk(const char *input, size_t input_size, char *output, size_t output_size,
                  const char *from_charset, const char *to_charset)
{
    iconv_t cd = iconv_open(to_charset, from_charset);
    if (cd == (iconv_t)-1)
    {
        perror("iconv_open for chunk");
        return -1;
    }

    char *inptr = (char *)input;
    char *outptr = output;
    size_t inbytes = input_size;
    size_t outbytes = output_size;

    if (iconv(cd, &inptr, &inbytes, &outptr, &outbytes) == (size_t)-1)
    {
        perror("iconv conversion for chunk");
        iconv_close(cd);
        return -1;
    }
    *outptr = '\0';
    iconv_close(cd);
    return 0;
}

// Step 2 & 3: Read the temporary single-byte file in 50-character chunks and convert each back to UTF-8.
void process_temp_file(const char *temp_filename)
{
    FILE *temp = fopen(temp_filename, "rb");
    if (!temp)
    {
        perror("Error opening temporary file");
        return;
    }

    char buffer[CHUNK_SIZE + 1];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, temp)) > 0)
    {
        buffer[bytes_read] = '\0';

        // Allocate sufficient space for UTF-8 conversion.
        char utf8_chunk[CHUNK_SIZE * 2];
        if (convert_chunk(buffer, bytes_read, utf8_chunk, sizeof(utf8_chunk),
                          "WINDOWS-1251", "UTF-8") != 0)
        {
            fprintf(stderr, "Error converting chunk to UTF-8\n");
            continue;
        }
        // ========================================================================================================================================
        // Perform operations on the chunk.
        // ========================================================================================================================================

        remove_mult_symbols(utf8_chunk, " ");
        remove_space_before_symbols(utf8_chunk, no_space_before_symbols);
        remove_mult_symbols(utf8_chunk, no_space_before_symbols);

        // ========================================================================================================================================
        // ========================================================================================================================================

        // Print the processed chunk.
        printf("%s", utf8_chunk);
    }
    fclose(temp);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <utf8_input_file>\n", argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *temp_filename = "temp_single_byte.txt";

    // Step 1: Convert the UTF-8 file to a single-byte encoding (WINDOWS-1251).
    if (convert_file_encoding(input_filename, temp_filename, "UTF-8", "WINDOWS-1251") != 0)
    {
        fprintf(stderr, "Conversion to single-byte encoding failed.\n");
        return 1;
    }

    // Step 2 & 3: Read the temporary file in fixed chunks, process each chunk,
    // and convert it back to UTF-8 for printing.
    printf("Processed output:\n");
    process_temp_file(temp_filename);
    printf("\n");

    return 0;
}

#ifndef READ_CONVERT_FILE_H
#define READ_CONVERT_FILE_H

#include <stdlib.h>

#define INITIAL_CHUNK_SIZE 50

void get_file_size(const char *input_file_name, size_t *buffer_size);
int convert_file_encoding(const char *input_filename, const char *output_filename,
                          const char *from_charset, const char *to_charset, size_t buffer_size);
int convert_chunk(const char *input, size_t input_size, char *output, size_t output_size,
                  const char *from_charset, const char *to_charset);
void process_temp_file(const char *temp_filename, const char *output_filename);

#endif // READ_CONVERT_FILE_H

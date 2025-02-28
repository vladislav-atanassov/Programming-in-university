#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 50

int read_file_to_buffer(const char *filepath, char *buffer, size_t buffer_size)
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    size_t bytesRead = fread(buffer, 1, buffer_size - 1, file);
    buffer[bytesRead] = '\0';
    fclose(file);
    return (int)bytesRead;
}

int main(int argc, char *argv[])
{
    char buffer[BUFFER_SIZE];

    int bytesRead = read_file_to_buffer("example.txt", buffer, BUFFER_SIZE);
    if (bytesRead >= 0)
    {
        printf("File contents:\n%s\n", buffer);
    }
    else
    {
        printf("Failed to read file.\n");
    }

    return 0;
}
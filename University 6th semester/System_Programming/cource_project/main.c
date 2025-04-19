#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

#include "text_highlighter.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s filename.txt\n", argv[0]);
        return 1;
    }

    setlocale(LC_ALL, "");

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("Error opening file");
        return 1;
    }

    char **lines = NULL;
    size_t num_lines = 0;
    size_t capacity = 0;
    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), fp))
    {
        if (num_lines >= capacity)
        {
            capacity = (capacity == 0) ? 100 : capacity * 2;
            lines = realloc(lines, capacity * sizeof(char *));
            if (!lines)
            {
                perror("Memory allocation failed");
                fclose(fp);
                return 1;
            }
        }
        lines[num_lines++] = strdup(buffer);
    }
    fclose(fp);

    int term_rows, term_cols;
    if (get_window_size(&term_rows, &term_cols) == -1)
    {
        term_rows = 24;
        term_cols = 80;
    }

    int display_lines = (PAGE_LINES < term_rows) ? PAGE_LINES : term_rows;
    int offset = 0;
    size_t cursor_x = 0, cursor_y = 0;

    enable_raw_mode();

    while (1)
    {
        clear_screen();

        for (int i = 0; i < display_lines; i++)
        {
            size_t line_index = (size_t)(offset + i);
            if (line_index >= num_lines)
                break;

            print_line_with_highlighting(lines[line_index], term_cols,
                                         offset + cursor_y, cursor_x, line_index);

            if (lines[line_index][strlen(lines[line_index]) - 1] != '\n')
                printf("\n");
        }

        fflush(stdout);
        enum Key key = wait_for_key();

        if (key == KEY_QUIT)
            break;
        else if (key == KEY_UP)
        {
            if (cursor_y > 0)
                cursor_y--;
            else if (offset > 0)
                offset--;
        }
        else if (key == KEY_DOWN)
        {
            if (cursor_y < (size_t)(display_lines - 1) && (size_t)(offset + cursor_y + 1) < num_lines)
                cursor_y++;
            else if ((size_t)(offset + display_lines) < num_lines)
                offset++;
        }
        else if (key == KEY_LEFT)
        {
            if (cursor_x > 0)
            {
                cursor_x--;
            }
            else if ((size_t)(offset + cursor_y) > 0)
            {
                size_t prev_line_index = (size_t)(offset + cursor_y - 1);

                if (cursor_y > 0)
                    cursor_y--;
                else if (offset > 0)
                    offset--;

                size_t prev_len = strlen(lines[prev_line_index]);
                if (prev_len > 0 && lines[prev_line_index][prev_len - 1] == '\n')
                    prev_len--;

                cursor_x = (prev_len > 0) ? prev_len - 1 : 0;
            }
        }
        else if (key == KEY_RIGHT)
        {
            size_t curr_line_index = (size_t)(offset + cursor_y);
            size_t curr_len = strlen(lines[curr_line_index]);

            wchar_t *wline = malloc((curr_len + 1) * sizeof(wchar_t));
            size_t num_chars = mbstowcs(wline, lines[curr_line_index], curr_len + 1);

            if (num_chars == (size_t)-1)
            {
                free(wline);
                return 1;
            }

            if (cursor_x < num_chars - 1)
            {
                cursor_x++;
            }
            else if (cursor_x == num_chars - 1)
            {
                if (curr_line_index + 1 < num_lines)
                {
                    cursor_y++;
                    cursor_x = 0;
                }
            }

            free(wline);
        }
        else if (key == KEY_CTRL_LEFT)
        {
            if (cursor_x > 0)
            {
                cursor_x = 0;
            }
            else if ((size_t)(offset + cursor_y) > 0)
            {
                if (cursor_y > 0)
                    cursor_y--;
                else if (offset > 0)
                    offset--;

                cursor_x = 0;
            }
        }
        else if (key == KEY_CTRL_RIGHT)
        {
            size_t curr_line_index = (size_t)(offset + cursor_y);
            size_t curr_len = strlen(lines[curr_line_index]);
            if (curr_len > 0 && lines[curr_line_index][curr_len - 1] == '\n')
                curr_len--;

            if (cursor_x < curr_len - 1)
            {
                cursor_x = curr_len - 1;
            }
            else if (curr_line_index + 1 < num_lines)
            {
                if (cursor_y < (size_t)(display_lines - 1) && (size_t)(offset + cursor_y + 1) < num_lines)
                    cursor_y++;
                else if ((size_t)(offset + display_lines) < num_lines)
                    offset++;

                size_t next_line_index = (size_t)(offset + cursor_y);
                size_t next_len = strlen(lines[next_line_index]);
                if (next_len > 0 && lines[next_line_index][next_len - 1] == '\n')
                    next_len--;

                cursor_x = (next_len > 0) ? next_len - 1 : 0;
            }
        }
        else if (key == KEY_PGDN)
        {
            offset += display_lines;
            if ((size_t)offset > num_lines - (size_t)display_lines)
                offset = (int)(num_lines - display_lines);
        }
        else if (key == KEY_PGUP)
        {
            offset -= display_lines;
            if (offset < 0)
                offset = 0;
        }
        else if (key == KEY_HOME)
        {
            offset = 0;
        }
        else if (key == KEY_END)
        {
            offset = (num_lines < (size_t)display_lines) ? 0 : (int)(num_lines - display_lines);
        }

        size_t line_len = strlen(lines[offset + cursor_y]);
        if (line_len > 0 && lines[offset + cursor_y][line_len - 1] == '\n')
            line_len--;

        if (cursor_x >= line_len)
            cursor_x = (line_len > 0) ? line_len - 1 : 0;
    }

    disable_raw_mode();

    for (size_t i = 0; i < num_lines; i++)
        free(lines[i]);
    free(lines);

    return 0;
}

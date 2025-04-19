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

struct termios orig_termios;

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int get_window_size(int *rows, int *cols)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
        return -1;
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
}

enum Key read_key()
{
    char c;
    char seq[6] = {0}; // Make sure it's zero-initialized
    int n = read(STDIN_FILENO, &c, 1);
    if (n == 0)
        return KEY_NONE;
    if (c == 'q')
        return KEY_QUIT;

    if (c == '\033') // ESC
    {
        // Try to read up to 5 more bytes
        for (int i = 0; i < 5; i++)
        {
            if (read(STDIN_FILENO, &seq[i], 1) != 1)
            {
                break;
            }
        }

        // Check known sequences
        if (seq[0] == '[')
        {
            if (seq[1] >= 'A' && seq[1] <= 'D')
            {
                switch (seq[1])
                {
                case 'A':
                    return KEY_UP;
                case 'B':
                    return KEY_DOWN;
                case 'C':
                    return KEY_RIGHT;
                case 'D':
                    return KEY_LEFT;
                }
            }

            if (seq[1] == '1' && seq[2] == ';' && seq[3] == '5')
            {
                if (seq[4] == 'C')
                    return KEY_CTRL_RIGHT;
                if (seq[4] == 'D')
                    return KEY_CTRL_LEFT;
            }

            if (seq[1] == '5' && seq[2] == '~')
                return KEY_PGUP;
            if (seq[1] == '6' && seq[2] == '~')
                return KEY_PGDN;
            if (seq[1] == 'H')
                return KEY_HOME;
            if (seq[1] == 'F')
                return KEY_END;
        }
        return KEY_OTHER;
    }

    return KEY_OTHER;
}

bool is_operator(char c)
{
    return (strchr(operators, c) != NULL);
}

bool is_keyword(const char *word)
{
    size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (size_t i = 0; i < num_keywords; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
            return true;
    }
    return false;
}

void print_line_with_highlighting(const char *line, int term_width,
                                  int cursor_line, int cursor_col, int current_line_index)
{
    mbstate_t state;
    memset(&state, 0, sizeof(state));

    const char *ptr = line;
    wchar_t wc;
    size_t bytes;
    int logical_col = 0;
    int visual_col = 0;
    bool in_string = false;
    char string_delim = '\0';
    bool in_comment = false;

    bool highlight_cursor = (current_line_index == cursor_line);

    while (*ptr != '\0')
    {
        bytes = mbrtowc(&wc, ptr, MB_CUR_MAX, &state);
        if (bytes == (size_t)-1 || bytes == (size_t)-2)
        {
            // Invalid sequence, fallback to single byte
            wc = (unsigned char)*ptr;
            bytes = 1;
        }

        bool cursor_here = highlight_cursor && (logical_col == cursor_col);

        // Line wrap
        if (visual_col >= term_width)
        {
            printf("\n");
            visual_col = 0;
        }

        // Handle newline
        if (wc == L'\n')
        {
            printf("\n");
            visual_col = 0;
            ptr += bytes;
            continue;
        }

        // Comment detection
        if (!in_string && wc == L'#' && !in_comment)
        {
            in_comment = true;
        }

        // Comment coloring
        if (in_comment)
        {
            if (cursor_here) printf(ANSI_CURSOR_REVERSE);
            printf(ANSI_COLOR_COMMENT "%.*s" ANSI_COLOR_RESET, (int)bytes, ptr);
            if (cursor_here) printf(ANSI_CURSOR_RESET);
            ptr += bytes;
            logical_col++;
            visual_col++;
            continue;
        }

        // String start
        if (!in_string && (wc == L'"' || wc == L'\''))
        {
            in_string = true;
            string_delim = (char)wc;
        }

        if (in_string)
        {
            if (cursor_here) printf(ANSI_CURSOR_REVERSE);
            printf(ANSI_COLOR_STRING "%.*s" ANSI_COLOR_RESET, (int)bytes, ptr);
            if (cursor_here) printf(ANSI_CURSOR_RESET);

            if (wc == string_delim)
                in_string = false;

            ptr += bytes;
            logical_col++;
            visual_col++;
            continue;
        }

        // Keywords and identifiers
        if (iswalpha(wc) || wc == L'_')
        {
            const char *start = ptr;
            int token_len = 0;
            char word_buf[64] = {0}; // buffer for token
            char *word_ptr = word_buf;

            // Read identifier
            while (*ptr)
            {
                mbstate_t temp = state;
                wchar_t next_wc;
                size_t b = mbrtowc(&next_wc, ptr, MB_CUR_MAX, &temp);
                if (b == (size_t)-1 || b == (size_t)-2 || !(iswalnum(next_wc) || next_wc == L'_'))
                    break;

                if (word_ptr - word_buf + b < 63)
                {
                    memcpy(word_ptr, ptr, b);
                    word_ptr += b;
                }

                ptr += b;
                token_len++;
            }

            *word_ptr = '\0';

            bool kw = is_keyword(word_buf);

            ptr = start;
            for (int j = 0; j < token_len; j++)
            {
                size_t b = mbrtowc(&wc, ptr, MB_CUR_MAX, &state);
                if (cursor_here) printf(ANSI_CURSOR_REVERSE);

                if (kw)
                    printf(ANSI_COLOR_KEYWORD "%.*s" ANSI_COLOR_RESET, (int)b, ptr);
                else
                    printf(ANSI_COLOR_NORMAL "%.*s" ANSI_COLOR_RESET, (int)b, ptr);

                if (cursor_here) printf(ANSI_CURSOR_RESET);

                ptr += b;
                logical_col++;
                visual_col++;
                cursor_here = highlight_cursor && (logical_col == cursor_col);
            }
            continue;
        }

        // Operators or misc
        if (cursor_here) printf(ANSI_CURSOR_REVERSE);

        if (is_operator((char)wc))
            printf(ANSI_COLOR_OPERATOR "%.*s" ANSI_COLOR_RESET, (int)bytes, ptr);
        else
            printf(ANSI_COLOR_NORMAL "%.*s" ANSI_COLOR_RESET, (int)bytes, ptr);

        if (cursor_here) printf(ANSI_CURSOR_RESET);

        ptr += bytes;
        logical_col++;
        visual_col++;
    }
}

void clear_screen()
{
    printf("\033[2J\033[H");
}

enum Key wait_for_key()
{
    enum Key key;
    do
    {
        key = read_key();
    } while (key == KEY_NONE);
    return key;
}

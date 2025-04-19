#ifndef TEXT_HIGHLIGHTER_H
#define TEXT_HIGHLIGHTER_H

#include <stdbool.h>

#define PAGE_LINES 30
#define MAX_LINE_LENGTH 1024

#define ANSI_COLOR_NORMAL    "\033[37m"
#define ANSI_COLOR_COMMENT   "\033[31m"
#define ANSI_COLOR_STRING    "\033[34m"
#define ANSI_COLOR_OPERATOR  "\033[36m"
#define ANSI_COLOR_KEYWORD   "\033[35m"
#define ANSI_COLOR_RESET     "\033[0m"
#define ANSI_CURSOR_REVERSE  "\033[7m"
#define ANSI_CURSOR_RESET    "\033[27m"

static const char *keywords[] = {
    "False", "None", "True", "and", "as", "assert", "async", "await",
    "break", "class", "continue", "def", "del", "elif", "else", "except",
    "finally", "for", "from", "global", "if", "import", "in", "is",
    "lambda", "nonlocal", "not", "or", "pass", "raise", "return",
    "try", "while", "with", "yield"
};

static const char *operators = "+-*/%=!&|^~<>:;.,()[]{}";

enum Key {
    KEY_NONE,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_PGUP,
    KEY_PGDN,
    KEY_HOME,
    KEY_END,
    KEY_QUIT,
    KEY_CTRL_LEFT,
    KEY_CTRL_RIGHT,
    KEY_OTHER
};

// === Terminal Control ===
void enable_raw_mode();
void disable_raw_mode();
int get_window_size(int *rows, int *cols);

// === Input Handling ===
enum Key read_key();
enum Key wait_for_key();

// === Syntax Highlighting ===
bool is_operator(char c);
bool is_keyword(const char *word);
void print_line_with_highlighting(const char *line, int term_width,
                                  int cursor_line, int cursor_col, int current_line_index);

// === Display ===
void clear_screen();

#endif // TEXT_HIGHLIGHTER_H

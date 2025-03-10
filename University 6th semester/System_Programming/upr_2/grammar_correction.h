#ifndef GRAMMAR_CORRECTION_H
#define GRAMMAR_CORRECTION_H

#define NO_SPACE_BEFORE_SYMBOLS "-,:;.!?"
#define END_OF_SENTENCE_SYMBOLS ".?!"

int is_symbol(char ch, const char *symbols);
void remove_mult_spaces(char *chunk);
void remove_space_before_symbols(char *chunk);
void adjust_dots(char *chunk);
void add_space_after_comma(char *chunk);
void remove_comma_from_end_of_sentance(char *chunk);
void fix_direct_speech_error(char *chunk);
void grammar_correction(char *chunk);

#endif // GRAMMAR_CORRECTION_H

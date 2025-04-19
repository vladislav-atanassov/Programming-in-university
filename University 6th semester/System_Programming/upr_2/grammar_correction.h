#ifndef GRAMMAR_CORRECTION_H
#define GRAMMAR_CORRECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colouring.h"
#include "dictionary.h"
#include "utf8.h"

// ======================================================================
// ============================== CONSTANTS =============================
// ======================================================================

#define TYPE_REMOVE -1
#define TYPE_ADD 1
#define TYPE_BEFORE -11
#define TYPE_AFTER 11
#define PUNCTUATION_SYMBOLS ",:.!?"
#define LETTERS "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЬЮЯабвгдежзийклмнопрстуфхцчшщъьюя"

// ======================================================================
// ============================== HELPERS ===============================
// ======================================================================

int *init_int_array(size_t array_size);
int adjust_indexing(int *corrections, int index);
void mark_for_correction(int *corrections, int index);
void remove_at_index(int *removals, char *chunk, size_t index);
void add_at_index(int *additions, char *chunk, const char *new_char, size_t index);
int is_symbol(const char ch, const char *symbols);
int search_for_string(const char *chunk, const char **ndgr_array,
                      int type_b_a);

// ======================================================================
// ============================ CORRECTIONS =============================
// ======================================================================

void remove_mult_spaces(int *corrections, char *chunk);
void remove_space_before_symbols(int *corrections, char *chunk);
void adjust_dots(int *corrections, char *chunk);
void remove_comma_from_punctuation(int *corrections, char *chunk);
void correct_direct_speech_error(int *corrections, char *chunk, int type_r_a);
void correct_comma_ndgr(int *removals, int *additions, char *chunk, const char **ndgr,
                        const int type_r_a, const int type_b_a);
void correct_comma_2_ndgr(int *removals, int *additions, char *chunk,
                          const char **ndgr_f, const char **ndgr_s, const int type_r_a);
void add_space_after_comma(int *corrections, char *chunk);

// ======================================================================
// ============================ MAIN LOGIC ==============================
// ======================================================================

void grammar_correction(char *corrected_chunk, char *chunk,
                        FILE *output_file, size_t chunk_size);

#endif // GRAMMAR_CORRECTION_H

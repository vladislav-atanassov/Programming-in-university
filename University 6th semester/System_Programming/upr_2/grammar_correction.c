#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar_correction.h"
#include "colouring.h"
#include "dictionary.h"
#include "utf8.h"

// ======================================================================
// ============================== HELPERS ===============================
// ======================================================================

// Initializes a heap allocated integer array filled with -1
int *init_int_array(size_t array_size)
{
    int *array = calloc(array_size, sizeof(int));
    if (NULL == array)
    {
        perror("Memory allocation error");

        free(array);
        array = NULL;
        return NULL;
    }
    memset(array, -1, array_size * sizeof(int));
    return array;
}

int adjust_indexing(int *corrections, int index)
{
    if (index == 0)
    {
        return 0;
    }

    int counter = 0;
    for (size_t i = 0; corrections[i] != -1; i++)
    {
        if (index > corrections[i])
        {
            counter++;
        }
    }
    return counter;
}

void mark_for_correction(int *corrections, int index)
{
    size_t i = 0;
    while (corrections[i] != -1)
    {
        i++;
    }
    corrections[i] = index;
}

void remove_at_index(int *removals, char *chunk, size_t index)
{
    size_t len = strlen(chunk);
    if (len < index)
    {
        return;
    }
    mark_for_correction(removals, index);
    size_t num_bytes = utf8_num_bytes(chunk + index);                               // Bytes to remove
    memmove(chunk + index, chunk + index + num_bytes, len - index - num_bytes + 1); // Shift left
}

void add_at_index(int *additions, char *chunk, const char *new_char, size_t index)
{
    size_t len = strlen(chunk);
    if (len < index)
    {
        return;
    }
    mark_for_correction(additions, index);
    size_t char_bytes = utf8_num_bytes((char *)new_char);
    memmove(chunk + index + char_bytes, chunk + index, len - index + 1); // +1 for null terminator
    memcpy(chunk + index, new_char, char_bytes);
}

int is_symbol(const char ch, const char *symbols)
{
    return (strchr(symbols, ch) != NULL);
}

int compare_strlen(const void *a, const void *b)
{
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;
    return strlen(str_b) - strlen(str_a);
}

// If substring is found:
// return index at eighter the start or the end of the first found substring.
// If substring is not found:
// return -1.
int search_for_string(const char *chunk, const char **ndgr_array, int type_b_a)
{
    size_t num_phrases = 0;
    while (ndgr_array[num_phrases] != NULL)
    {
        num_phrases++;
    }

    const char **sorted_ndgr = malloc((num_phrases + 1) * sizeof(const char *));
    if (NULL == sorted_ndgr)
    {
        perror("Memory allocation error");

        free(sorted_ndgr);
        sorted_ndgr = NULL;
        return -100;
    }

    memcpy(sorted_ndgr, ndgr_array, num_phrases * sizeof(const char *));
    sorted_ndgr[num_phrases] = NULL;
    // Sort phrases by length (longest first)
    qsort(sorted_ndgr, num_phrases, sizeof(const char *), compare_strlen);

    int found_index = -1;
    for (size_t i = 0; sorted_ndgr[i] != NULL; i++)
    {
        const char *phrase = sorted_ndgr[i];
        const char *pos = strstr(chunk, phrase);
        if (pos != NULL)
        {
            int begin = (int)(pos - chunk);
            int end = begin + strlen(phrase);

            // Check if this phrase overlaps with a previously matched longer phrase
            if (found_index != -1 && (begin <= found_index || end <= found_index))
            {
                continue; // Skip shorter overlapping phrases
            }

            int is_word_boundary_before = (begin == 0) || !is_symbol(chunk[begin - 1], LETTERS);
            int is_word_boundary_after = (chunk[end] == '\0') || !is_symbol(chunk[end], LETTERS);

            if (is_word_boundary_before && is_word_boundary_after)
            {
                found_index = (type_b_a == TYPE_BEFORE) ? begin : end;
                break; // Prioritize the longest valid phrase
            }
        }
    }

    free(sorted_ndgr);
    return found_index;
}

//! When adding before it marks the first letter of the phrase too
int utf8_prev_index(const char *chunk, int index)
{
    if (index <= 0)
        return 0;
    int i = index - 1;
    // While this byte is a continuation byte (bits 10xxxxxx), back up.
    while (i > 0 && (((unsigned char)chunk[i] & 0xC0) == 0x80))
        i--;
    return i;
}

// ======================================================================
// ============================== REMOVALS ==============================
// ======================================================================

void remove_mult_spaces(int *corrections, char *chunk)
{
    int in_space = 0;
    for (size_t i = 0; chunk[i] != '\0';)
    {
        if (chunk[i] == ' ')
        {
            if (!in_space)
            {
                in_space = 1;
                i++;
            }
            else
            {
                // Don't increment i, re-check the current index after removal
                remove_at_index(corrections, chunk, i);
            }
        }
        else
        {
            in_space = 0;
            i++;
        }
    }
}

void remove_space_before_symbols(int *corrections, char *chunk)
{
    for (size_t i = 1; chunk[i] != '\0'; i++) // Start from i = 1 to prevend error
    {
        if (is_symbol(chunk[i], PUNCTUATION_SYMBOLS) && (chunk[i - 1] == ' '))
        {
            remove_at_index(corrections, chunk, i - 1);
        }
    }
}

void adjust_dots(int *corrections, char *chunk)
{
    size_t dots_count = 0;
    for (size_t i = 0; chunk[i] != '\0';)
    {
        if (chunk[i] == '.')
        {
            dots_count++;

            if (dots_count > 3)
            {
                // Don't increment i, check same position
                remove_at_index(corrections, chunk, i);
            }
            else
            {
                i++; // Only increment if no removal
            }
        }
        else
        {
            if (dots_count == 2)
            {
                remove_at_index(corrections, chunk, i - 1);
                i--;
            }
            dots_count = 0;
            i++;
        }
    }
    size_t len = strlen(chunk);
    if (dots_count == 2 && len >= 2)
    { // Handle trailing ".."
        remove_at_index(corrections, chunk, len - 1);
    }
}

void remove_comma_from_punctuation(int *corrections, char *chunk)
{
    for (size_t i = 0; chunk[i] != '\0';)
    {
        if (chunk[i] == ',')
        {
            if (i > 0 && is_symbol(chunk[i - 1], PUNCTUATION_SYMBOLS))
            {
                remove_at_index(corrections, chunk, i);
            }
            if (is_symbol(chunk[i + 1], PUNCTUATION_SYMBOLS))
            {
                remove_at_index(corrections, chunk, i);
            }
        }
        i++;
    }
}

void correct_direct_speech_error(int *corrections, char *chunk, int type_r_a)
{
    for (size_t i = 0; chunk[i] != '\0'; i++)
    {
        if (TYPE_REMOVE == type_r_a)
        {
            if (chunk[i] == '\n' &&
                chunk[i - 1] == ' ' &&
                chunk[i - 2] == ',')
            {
                remove_at_index(corrections, chunk, i);
            }
        }
        else if (TYPE_ADD == type_r_a)
        {
            if (chunk[i] == '-' &&
                chunk[i + 1] != ' ')
            {
                add_at_index(corrections, chunk, " ", i + 1);
            }
        }
    }
}

void correct_comma_ndgr(int *removals, int *additions, char *chunk,
                        const char **ndgr, const int type_r_a, const int type_b_a)
{
    int index_if_found = search_for_string(chunk, ndgr, type_b_a);
    if (index_if_found == -1)
    {
        return;
    }

    size_t phrase_len = strlen(ndgr[0]);

    for (size_t i = 0; ndgr[i] != NULL; i++)
    {
        if (strlen(ndgr[i]) > phrase_len && strstr(chunk, ndgr[i]) != NULL)
        {
            return;
        }
    }

    if ((index_if_found >= 2) && (TYPE_BEFORE == type_b_a))
    {
        int prev = utf8_prev_index(chunk, index_if_found);

        if (TYPE_REMOVE == type_r_a)
        {
            if (chunk[prev] == ' ')
            {
                if (chunk[prev - 1] == ',')
                {
                    remove_at_index(removals, chunk, prev - 1);
                }
                else
                {
                    remove_at_index(removals, chunk, prev);
                    add_at_index(additions, chunk, " ", prev);
                }
            }
        }
        else if (TYPE_ADD == type_r_a)
        {
            if ((prev >= 0 && memcmp(chunk + prev, ",", 1) == 0) ||
                (prev >= 0 && memcmp(chunk + prev - 1, ",", 1) == 0))
            {
                return;
            }
            else
            {
                add_at_index(additions, chunk, ",", prev);
            }
        }
    }
    else if ((index_if_found + 2 <= (int)strlen(chunk)) && (TYPE_AFTER == type_b_a))
    {
        if (TYPE_REMOVE == type_r_a)
        {
            if (memcmp(chunk + index_if_found, ",", 1) == 0)
            {
                if (index_if_found > 0 && memcmp(chunk + index_if_found - 1, " ", 1) == 0)
                {
                    return;
                }
                else
                {
                    remove_at_index(removals, chunk, index_if_found);
                    add_at_index(additions, chunk, " ", index_if_found);
                }
            }
        }
        else if (TYPE_ADD == type_r_a)
        {
            if (memcmp(chunk + index_if_found, ",", 1) == 0)
            {
                return;
            }
            else
            {
                add_at_index(additions, chunk, ",", index_if_found);
            }
        }
    }

    // TODO: Think about this solution
    // // func_arg -> size_t start_index = 0
    // for (size_t start_index = 0; chunk[start_index] != '\0' /* 1. || start_index != 0 */; start_index++)
    // {
    //     // .
    //     // .
    //     // .
    //     // Eighter of 1. or 2. statement 
    //     if (start_index != 0) // 2.
    //     {
    //         return;
    //     }
    // }
}

void correct_comma_2_ndgr(int *removals, int *additions, char *chunk,
                          const char **ndgr_f, const char **ndgr_s, const int type_r_a)
{
    int index_if_found_f, index_if_found_s;

    for (size_t i = 0; chunk[i] != '\0'; i += index_if_found_s)
    {
        index_if_found_f = search_for_string(chunk, ndgr_f, TYPE_AFTER);
        index_if_found_s = search_for_string(chunk, ndgr_s, TYPE_BEFORE);

        if (index_if_found_f == -1 || index_if_found_s == -1 || (index_if_found_s - index_if_found_f) > 2)
        {
            break;
        }

        correct_comma_ndgr(removals, additions, chunk, ndgr_s, type_r_a, TYPE_BEFORE);
    }
}

// ======================================================================
// ============================== ADDITIONS =============================
// ======================================================================

// TODO: Add a stament: if there is space after the comma do nothing
void add_space_after_comma(int *corrections, char *chunk)
{
    for (size_t i = 0; chunk[i] != '\0'; i++)
    {
        if (chunk[i] == ',' &&
            chunk[i + 1] != ' ' &&
            chunk[i + 1] != '\n')
        {
            mark_for_correction(corrections, i + 1);
            add_at_index(corrections, chunk, " ", i + 1);
        }
    }
}

void adjust_v_s(int *additions, char *chunk, const char **ndgr_v_s)
{
    int offset = 0;
    while (1)
    {
        int pos_after = search_for_string(chunk + offset, ndgr_v_s, TYPE_AFTER);
        if (pos_after == -1)
        {
            break;
        }

        pos_after += offset;
        int prepos_start = pos_after - 2;
        if (memcmp(chunk + prepos_start, "в", 2) == 0)
        {
            if (chunk[pos_after] == ' ')
            {
                if (memcmp(chunk + pos_after + 1, "в", 2) == 0 ||
                    memcmp(chunk + pos_after + 1, "ф", 2) == 0)
                {
                    add_at_index(additions, chunk, "в", pos_after);
                    add_at_index(additions, chunk, "ъ", pos_after);
                    offset = prepos_start + 2 + 2 + 2;
                    continue;
                }
            }
        }
        else if (memcmp(chunk + prepos_start, "с", 2) == 0)
        {
            if (chunk[pos_after] == ' ')
            {
                if (memcmp(chunk + pos_after + 1, "с", 2) == 0 ||
                    memcmp(chunk + pos_after + 1, "з", 2) == 0)
                {
                    add_at_index(additions, chunk, "с", pos_after);
                    add_at_index(additions, chunk, "ъ", pos_after);
                    offset = prepos_start + 2 + 2 + 2;
                    continue;
                }
            }
        }
        offset = pos_after;
    }
}

void grammar_correction(char *corrected_chunk, char *chunk, FILE *output_file, size_t chunk_size)
{
    int *removals = init_int_array(chunk_size);
    int *additions = init_int_array(chunk_size);

    if ((NULL == removals) || (NULL == additions))
    {
        return;
    }

    // ==================================================================
    // ============================ REMOVALS ============================
    // ==================================================================
    // Rule 3
    correct_direct_speech_error(removals, corrected_chunk, TYPE_REMOVE);
    // printf("After correct_direct_speech_error removal: %s", corrected_chunk);
    // Rule 12
    remove_mult_spaces(removals, corrected_chunk);
    // printf("After remove_mult_spaces: %s", corrected_chunk);
    // Rule 1, 4, 5, 8 (4 and 8 are the same)
    remove_space_before_symbols(removals, corrected_chunk);
    // printf("After remove_space_before_symbols: %s", corrected_chunk);
    // Rule 2
    adjust_dots(removals, corrected_chunk);
    // printf("After adjust_dots: %s", corrected_chunk);
    // Rule 6
    remove_comma_from_punctuation(removals, corrected_chunk);
    // printf("After remove_comma_from_punctuation: %s", corrected_chunk);
    // Rule 9
    correct_comma_ndgr(removals, additions, corrected_chunk, ndgr1, TYPE_REMOVE, TYPE_AFTER);
    // printf("After correct_comma_ndgr...ndgr1, TYPE_REMOVE, TYPE_AFTER): %s", corrected_chunk);
    // Rule 13
    correct_comma_ndgr(removals, additions, corrected_chunk, ndgr4, TYPE_REMOVE, TYPE_AFTER);
    // printf("After correct_comma_ndgr...ndgr4, TYPE_REMOVE, TYPE_AFTER): %s", corrected_chunk);
    // Rule 14
    correct_comma_ndgr(removals, additions, corrected_chunk, ndgr5, TYPE_REMOVE, TYPE_AFTER);
    // printf("After correct_comma_ndgr...ndgr5, TYPE_REMOVE, TYPE_AFTER): %s", corrected_chunk);
    // Rule 15
    correct_comma_2_ndgr(removals, additions, corrected_chunk, ndgr7, ndgr6, TYPE_REMOVE);
    // printf("After correct_2_comma_ndgr...ndgr7, ndgr6, TYPE_REMOVE): %s", corrected_chunk);
    // Rule 16
    correct_comma_2_ndgr(removals, additions, corrected_chunk, ndgr_ne, ndgr_zashtoto, TYPE_REMOVE);
    correct_comma_2_ndgr(removals, additions, corrected_chunk, ndgr5, ndgr_zashtoto, TYPE_REMOVE);
    // printf("After correct_2_comma_ndgr...ndgr5/ndgr_ne, ndgr_zashtoto, TYPE_REMOVE): %s", corrected_chunk);
    // Rule 18
    correct_comma_2_ndgr(removals, additions, corrected_chunk, ndgr9, ndgr10, TYPE_REMOVE);
    // printf("After correct_2_comma_ndgr...ndgr9, ndgr10, TYPE_REMOVE): %s", corrected_chunk);
    // Rule 19
    correct_comma_2_ndgr(removals, additions, corrected_chunk, ndgr11, ndgr_che, TYPE_REMOVE);
    // printf("After correct_2_comma_ndgr...ndgr11, ndgr_che, TYPE_REMOVE): %s", corrected_chunk);
    // Rule 20
    correct_comma_2_ndgr(removals, additions, corrected_chunk, ndgr_che, ndgr_za_da, TYPE_REMOVE);
    // printf("After correct_2_comma_ndgr...ndgr_che, ndgr_za_da, TYPE_REMOVE): %s", corrected_chunk);
    // Rule 21
    correct_comma_2_ndgr(removals, additions, corrected_chunk, ndgr12, ndgr_kato, TYPE_REMOVE);
    // printf("After correct_2_comma_ndgr...ndgr12, ndgr_kato, TYPE_REMOVE): %s", corrected_chunk);
    // Rule 23
    correct_comma_2_ndgr(removals, additions, corrected_chunk, ndgr13, ndgr_estestveno, TYPE_REMOVE);
    // printf("After correct_2_comma_ndgr...ndgr13, ndgr_estestveno, TYPE_REMOVE): %s", corrected_chunk);

    highlight_to_file(removals, chunk, output_file, TYPE_REMOVE);

    // ===================================================================
    // ============================ ADDITIONS ============================
    // ===================================================================
    correct_direct_speech_error(additions, corrected_chunk, TYPE_ADD);
    // printf("After correct_direct_speech_error addition: %s", corrected_chunk);

    // Rule 10
    correct_comma_ndgr(removals, additions, corrected_chunk, ndgr2, TYPE_ADD, TYPE_AFTER);
    // printf("After correct_comma_ndgr...ndgr2, TYPE_ADD, TYPE_AFTER): %s", corrected_chunk);
    // Rule 11
    correct_comma_ndgr(removals, additions, corrected_chunk, ndgr3, TYPE_ADD, TYPE_BEFORE);
    correct_comma_ndgr(removals, additions, corrected_chunk, ndgr3, TYPE_ADD, TYPE_AFTER);
    // printf("After correct_comma_ndgr...ndgr3, TYPE_ADD, TYPE_BEFORE/TYPE_AFTER): %s", corrected_chunk);
    // Rule 17
    correct_comma_ndgr(removals, additions, corrected_chunk, ndgr_sorry, TYPE_ADD, TYPE_BEFORE);
    correct_comma_ndgr(removals, additions, corrected_chunk, ndgr_sorry, TYPE_ADD, TYPE_AFTER);
    // printf("After correct_comma_ndgr...ndgr_sorry, TYPE_ADD, TYPE_BEFORE/TYPE_AFTER): %s", corrected_chunk);
    // Rule 22
    correct_comma_ndgr(removals, additions, corrected_chunk, ndgr13, TYPE_ADD, TYPE_BEFORE);
    // printf("After correct_comma_ndgr...ndgr13, TYPE_ADD, TYPE_BEFORE): %s", corrected_chunk);
    // Rule 24, 25
    adjust_v_s(additions, corrected_chunk, ndgr_v_s);
    // printf("After adjust_v_s: %s", corrected_chunk);
    // Rule 26
    correct_comma_ndgr(removals, additions, corrected_chunk, ndgr8, TYPE_ADD, TYPE_BEFORE);
    // printf("After correct_comma_ndgr...ndgr8, TYPE_ADD, TYPE_BEFORE): %s", corrected_chunk);

    // Remove if necessary
    remove_mult_spaces(removals, corrected_chunk);
    remove_space_before_symbols(removals, corrected_chunk);
    remove_comma_from_punctuation(removals, corrected_chunk);
    // Rule 7
    add_space_after_comma(additions, corrected_chunk);
    // printf("After add_space_after_comma: %s", corrected_chunk);

    highlight_to_file(additions, corrected_chunk, output_file, TYPE_ADD);

    free(removals);
    free(additions);
    removals = NULL;
    additions = NULL;
}
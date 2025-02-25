#include <stdio.h>
#include <stdlib.h>   // EXIT_FAILURE
#include <sys/mman.h> // mmap(), munmap()
#include <fcntl.h>    // open(), close(), O_RDONLY
#include <string.h>   // strerror()
#include <unistd.h>   // lseek()
#include <errno.h>

#define START_STATE 0
#define IN_WORD_STATE 1
#define ERROR_STATE -1
#define END_STATE 27
#define PLUS_SIGN_STATE 2
#define MINUS_SIGN_STATE 3
#define P_ZERO_STATE 4
#define M_ZERO_STATE 5
#define N_S_ZERO_STATE 6
#define FRAC_CONST_STATE 7
#define U_INT_STATE 8
#define S_INT_STATE 9
#define FIR_U_SUFFIX_STATE 10
#define SEC_U_SUFFIX_STATE 11
// TODO: Create the states for the small and capital 'l' for catching the case with "lL" and "Ll" as an ERROR in the state machine
// TODO: ========================================================================================================================
// TODO: ========================================================================================================================
#define S_S_L_SUFFIX_STATE 12
#define U_S_L_SUFFIX_STATE 13
#define L_S_L_SUFFIX_STATE 14
#define S_C_L_SUFFIX_STATE 15
#define U_C_L_SUFFIX_STATE 16
#define L_C_L_SUFFIX_STATE 17
// TODO: ========================================================================================================================
// TODO: ========================================================================================================================
#define OCTAL_STATE 18
#define HEX_STATE 19
#define DOUBLE_STATE 20
#define F_SUFFIX_STATE 21
#define EXPONENT_STATE 22
#define S_EXPONENT_STATE 23
#define F_NUM_SUFFIXES_STATE 24
#define E_DOUBLE_STATE 25
#define L_D_SUFFIX_STATE 26

#define END_OF_BUFFER '\0'

#define SIZE_DELIMITERS 6
#define SIZE_DIGITS 11
#define SIZE_NON_ZERO_DIGITS 10
#define SIZE_OCTAL_CONSTS 9
#define SIZE_HEX_CONSTS 23

const char delimiters[] = {' ', ',', '\r', '\n', EOF, '\0'};
const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};
const char non_zero_digits[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};
const char octal_consts[] = {'0', '1', '2', '3', '4', '5', '6', '7', '\0'};
const char hex_consts[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                           'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F', '\0'};

#define SIZE_NUMS_TYPE 8
#define INDEX_IS_VALID_NUM 0
#define INDEX_IS_SIGNED 1
#define INDEX_IS_WHOLE_NUM 2
#define INDEX_IS_OCTAL 3
#define INDEX_IS_HEXIDECIMAL 4
#define INDEX_IS_FLOAT 5
#define INDEX_IS_REAL_NUM 6
#define INDEX_IS_U_WHOLE_NUM 7

int nums_type_flags[SIZE_NUMS_TYPE] = {0, 0, 0, 0, 0, 0, 0, 0};
int nums_type_counters[SIZE_NUMS_TYPE] = {0, 0, 0, 0, 0, 0, 0, 0};

void reset_type_count()
{
    nums_type_flags[INDEX_IS_VALID_NUM] = 0;
    nums_type_flags[INDEX_IS_SIGNED] = 0;
    nums_type_flags[INDEX_IS_WHOLE_NUM] = 0;
    nums_type_flags[INDEX_IS_OCTAL] = 0;
    nums_type_flags[INDEX_IS_HEXIDECIMAL] = 0;
    nums_type_flags[INDEX_IS_FLOAT] = 0;
    nums_type_flags[INDEX_IS_REAL_NUM] = 0;
    nums_type_flags[INDEX_IS_U_WHOLE_NUM] = 0;

    return;
}

void increment_type_count()
{
    for (int i = 0; i < SIZE_NUMS_TYPE; i++)
    {
        if (nums_type_flags[i] == 1)
        {
            nums_type_counters[i]++;
        }
    }

    return;
}

int is_delimiter(char ch)
{
    for (int i = 0; i < SIZE_DELIMITERS - 1; i++)
    {
        if (ch == delimiters[i])
        {
            return 1;
        }
    }

    return 0;
}

int is_digit(char ch)
{
    for (int i = 0; i < SIZE_DIGITS - 1; i++)
    {
        if (ch == digits[i])
        {
            return 1;
        }
    }

    return 0;
}

int is_non_zero_digit(char ch)
{
    for (int i = 0; i < SIZE_NON_ZERO_DIGITS - 1; i++)
    {
        if (ch == non_zero_digits[i])
        {
            return 1;
        }
    }

    return 0;
}

int is_octal_const(char ch)
{
    for (int i = 0; i < SIZE_OCTAL_CONSTS - 1; i++)
    {
        if (ch == octal_consts[i])
        {
            return 1;
        }
    }

    return 0;
}

int is_hex_const(char ch)
{
    for (int i = 0; i < SIZE_HEX_CONSTS - 1; i++)
    {
        if (ch == hex_consts[i])
        {
            return 1;
        }
    }

    return 0;
}

int is_unsigned_suffix(char ch)
{
    return ((ch == 'u') || (ch == 'U'));
}

int is_long_suffix(char ch)
{
    return ((ch == 'l') || (ch == 'L'));
}

int is_small_long_suffix(char ch)
{
    return (ch == 'l');
}

int is_capital_long_suffix(char ch)
{
    return (ch == 'L');
}

int is_float_suffix(char ch)
{
    return ((ch == 'f') || (ch == 'F'));
}

int is_exponent(char ch)
{
    return ((ch == 'e') || (ch == 'E'));
}

int is_hex_prefix(char ch)
{
    return ((ch == 'x') || (ch == 'X'));
}

void numbers_identificator(char *buffer)
{
    int curr_state = START_STATE;
    int i = 0;
    int errors = 0;

    while (1)
    {
        switch (curr_state)
        {
        case START_STATE:
            if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = IN_WORD_STATE;
            }

            break;
        case IN_WORD_STATE:
            // printf("Current state: IN_WORD_STATE.\t\tCurrent char: %c\n", buffer[i]);

            increment_type_count();
            reset_type_count();

            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == '+')
            {
                curr_state = PLUS_SIGN_STATE;
            }
            else if (buffer[i] == '-')
            {
                curr_state = MINUS_SIGN_STATE;
            }
            else if (buffer[i] == '0')
            {
                curr_state = N_S_ZERO_STATE;
            }
            else if (buffer[i] == '.')
            {
                curr_state = FRAC_CONST_STATE;
            }
            else if (is_non_zero_digit(buffer[i]))
            {
                curr_state = U_INT_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case ERROR_STATE:
            // printf("Current state: ERROR_STATE.\t\tCurrent char: %c\n", buffer[i]);

            reset_type_count();

            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
                errors++;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case END_STATE:
            // printf("Current state: END_STATE.\t\tCurrent char: %c\n", buffer[i]);
            increment_type_count();

            printf("IS_VALID_NUM:\t\t%d\n", nums_type_counters[INDEX_IS_VALID_NUM]);
            printf("IS_WHOLE_NUM:\t\t%d\n", nums_type_counters[INDEX_IS_WHOLE_NUM]);
            printf("IS_REAL_NUM:\t\t%d\n", nums_type_counters[INDEX_IS_REAL_NUM]);
            printf("IS_U_WHOLE_NUM:\t\t%d\n", nums_type_counters[INDEX_IS_U_WHOLE_NUM]);
            printf("IS_OCTAL:\t\t%d\n", nums_type_counters[INDEX_IS_OCTAL]);
            printf("IS_HEXIDECIMAL:\t\t%d\n", nums_type_counters[INDEX_IS_HEXIDECIMAL]);
            printf("IS_FLOAT:\t\t%d\n", nums_type_counters[INDEX_IS_FLOAT]);

            // printf("IS_SIGNED:\t\t%d\n", nums_type_counters[INDEX_IS_SIGNED]);
            // printf("IS_ERROR:\t\t%d\n", errors);
            // printf("TOTAL WORDS:\t\t%d\n", nums_type_counters[INDEX_IS_VALID_NUM] + errors);

            return;

        case PLUS_SIGN_STATE:
            // printf("Current state: PLUS_SIGN_STATE.\t\tCurrent char: %c\n", buffer[i]);
            if (buffer[i] == '0')
            {
                curr_state = P_ZERO_STATE;
            }
            else if (is_non_zero_digit(buffer[i]))
            {
                curr_state = U_INT_STATE;
            }
            else if (buffer[i] == '.')
            {
                curr_state = FRAC_CONST_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case MINUS_SIGN_STATE:
            // printf("Current state: MINUS_SIGN_STATE.\t\tCurrent char: %c\n", buffer[i]);

            if (buffer[i] == '0')
            {
                curr_state = M_ZERO_STATE;
            }
            else if (is_non_zero_digit(buffer[i]))
            {
                curr_state = S_INT_STATE;
            }
            else if (buffer[i] == '.')
            {
                curr_state = FRAC_CONST_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        // TODO: Change U_ZERO_STATE to the current P_ZERO_STATE and adjust the lines accordingly in the state machine
        case P_ZERO_STATE:
            // printf("Current state: P_ZERO_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_VALID_NUM] = 1;
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 1;
            nums_type_flags[INDEX_IS_U_WHOLE_NUM] = 1;

            if (buffer[i] == '.')
            {
                curr_state = FRAC_CONST_STATE;
            }
            else if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (is_exponent(buffer[i]))
            {
                curr_state = EXPONENT_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        // TODO: Change S_ZERO_STATE to the current M_ZERO_STATE in the state machine
        case M_ZERO_STATE:
            // printf("Current state: M_ZERO_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_VALID_NUM] = 1;
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 1;

            if (buffer[i] == '.')
            {
                curr_state = FRAC_CONST_STATE;
            }
            else if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (is_exponent(buffer[i]))
            {
                curr_state = EXPONENT_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        // TODO: Add the new state N_S_ZERO_STATE to the state machine graph
        case N_S_ZERO_STATE:
            // printf("Current state: N_S_ZERO_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_VALID_NUM] = 1;
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 1;
            nums_type_flags[INDEX_IS_U_WHOLE_NUM] = 1;

            if (is_hex_prefix(buffer[i]))
            {
                curr_state = HEX_STATE;
            }
            else if (is_octal_const(buffer[i]))
            {
                curr_state = OCTAL_STATE;
            }
            else if (is_unsigned_suffix(buffer[i]))
            {
                curr_state = FIR_U_SUFFIX_STATE;
            }
            else if (is_small_long_suffix(buffer[i]))
            {
                curr_state = U_S_L_SUFFIX_STATE;
            }
            else if (is_capital_long_suffix(buffer[i]))
            {
                curr_state = U_C_L_SUFFIX_STATE;
            }
            else if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (is_exponent(buffer[i]))
            {
                curr_state = EXPONENT_STATE;
            }
            else if (buffer[i] == '.')
            {
                curr_state = FRAC_CONST_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case FRAC_CONST_STATE:
            // printf("Current state: FRAC_CONST_STATE.\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 0;
            nums_type_flags[INDEX_IS_U_WHOLE_NUM] = 0;
            nums_type_flags[INDEX_IS_REAL_NUM] = 1;

            if (is_digit(buffer[i]))
            {
                curr_state = DOUBLE_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                reset_type_count();
                curr_state = END_STATE;
            }
            else if (is_exponent(buffer[i]))
            {
                curr_state = EXPONENT_STATE;
            }
            else if (is_float_suffix(buffer[i]))
            {
                curr_state = F_SUFFIX_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case U_INT_STATE:
            // printf("Current state: U_INT_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_VALID_NUM] = 1;
            nums_type_flags[INDEX_IS_U_WHOLE_NUM] = 1;
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 1;

            if (is_digit(buffer[i]))
            {
                curr_state = U_INT_STATE;
            }
            else if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (is_unsigned_suffix(buffer[i]))
            {
                curr_state = FIR_U_SUFFIX_STATE;
            }
            else if (is_small_long_suffix(buffer[i]))
            {
                curr_state = U_S_L_SUFFIX_STATE;
            }
            else if (is_capital_long_suffix(buffer[i]))
            {
                curr_state = U_C_L_SUFFIX_STATE;
            }
            else if (buffer[i] == '.')
            {
                curr_state = FRAC_CONST_STATE;
            }
            else if (is_exponent(buffer[i]))
            {
                curr_state = EXPONENT_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case S_INT_STATE:
            // printf("Current state: S_INT_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_VALID_NUM] = 1;
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 1;

            if (is_digit(buffer[i]))
            {
                curr_state = S_INT_STATE;
            }
            else if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (is_small_long_suffix(buffer[i]))
            {
                curr_state = U_S_L_SUFFIX_STATE;
            }
            else if (is_capital_long_suffix(buffer[i]))
            {
                curr_state = U_C_L_SUFFIX_STATE;
            }
            else if (is_exponent(buffer[i]))
            {
                curr_state = EXPONENT_STATE;
            }
            else if (buffer[i] == '.')
            {
                curr_state = FRAC_CONST_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case FIR_U_SUFFIX_STATE:
            // printf("Current state: FIR_U_SUFFIX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            if (is_small_long_suffix(buffer[i]))
            {
                curr_state = U_S_L_SUFFIX_STATE;
            }
            else if (is_capital_long_suffix(buffer[i]))
            {
                curr_state = U_C_L_SUFFIX_STATE;
            }
            else if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case SEC_U_SUFFIX_STATE:
            // printf("Current state: SEC_U_SUFFIX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;
            break;
        case S_S_L_SUFFIX_STATE:
            // printf("Current state: S_L_SUFFIX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else if (is_small_long_suffix(buffer[i]))
            {
                curr_state = L_S_L_SUFFIX_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case S_C_L_SUFFIX_STATE:
            // printf("Current state: S_L_SUFFIX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else if (is_capital_long_suffix(buffer[i]))
            {
                curr_state = L_C_L_SUFFIX_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case U_S_L_SUFFIX_STATE:
            // printf("Current state: U_L_SUFFIX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else if (is_small_long_suffix(buffer[i]))
            {
                curr_state = L_S_L_SUFFIX_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case U_C_L_SUFFIX_STATE:
            // printf("Current state: U_L_SUFFIX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else if (is_capital_long_suffix(buffer[i]))
            {
                curr_state = L_C_L_SUFFIX_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case L_S_L_SUFFIX_STATE:
            // printf("Current state: L_LSUFFIX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else if (is_unsigned_suffix(buffer[i]))
            {
                curr_state = SEC_U_SUFFIX_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case L_C_L_SUFFIX_STATE:
            // printf("Current state: L_LSUFFIX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else if (is_unsigned_suffix(buffer[i]))
            {
                curr_state = SEC_U_SUFFIX_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case OCTAL_STATE:
            // printf("Current state: OCTAL_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_VALID_NUM] = 1;
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 1;
            nums_type_flags[INDEX_IS_U_WHOLE_NUM] = 1;
            nums_type_flags[INDEX_IS_OCTAL] = 1;

            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (is_octal_const(buffer[i]))
            {
                curr_state = OCTAL_STATE;
            }
            else if (is_exponent(buffer[i]))
            {
                curr_state = EXPONENT_STATE;
            }
            else if (is_small_long_suffix(buffer[i]))
            {
                curr_state = U_S_L_SUFFIX_STATE;
            }
            else if (is_capital_long_suffix(buffer[i]))
            {
                curr_state = U_C_L_SUFFIX_STATE;
            }
            else if (is_unsigned_suffix(buffer[i]))
            {
                curr_state = FIR_U_SUFFIX_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case HEX_STATE:
            // printf("Current state: HEX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_VALID_NUM] = 1;
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 1;
            nums_type_flags[INDEX_IS_U_WHOLE_NUM] = 1;
            nums_type_flags[INDEX_IS_HEXIDECIMAL] = 1;

            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (is_hex_const(buffer[i]))
            {
                curr_state = HEX_STATE;
            }
            else if (is_small_long_suffix(buffer[i]))
            {
                curr_state = U_S_L_SUFFIX_STATE;
            }
            else if (is_capital_long_suffix(buffer[i]))
            {
                curr_state = U_C_L_SUFFIX_STATE;
            }
            else if (is_unsigned_suffix(buffer[i]))
            {
                curr_state = FIR_U_SUFFIX_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case DOUBLE_STATE:
            // printf("Current state: DOUBLE_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_VALID_NUM] = 1;
            nums_type_flags[INDEX_IS_REAL_NUM] = 1;
            nums_type_flags[INDEX_IS_U_WHOLE_NUM] = 0;
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 0;

            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (is_digit(buffer[i]))
            {
                curr_state = DOUBLE_STATE;
            }
            else if (is_float_suffix(buffer[i]))
            {
                curr_state = F_SUFFIX_STATE;
            }
            else if (is_long_suffix(buffer[i]))
            {
                curr_state = L_D_SUFFIX_STATE;
            }
            else if (is_exponent(buffer[i]))
            {
                curr_state = EXPONENT_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case F_SUFFIX_STATE:
            // printf("Current state: F_SUFFIX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_REAL_NUM] = 1;
            nums_type_flags[INDEX_IS_FLOAT] = 1;

            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        // TODO: Add a line with '+' to the new S_EXPONENT_STATE
        case EXPONENT_STATE:
            // printf("Current state: EXPONENT_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 0;
            nums_type_flags[INDEX_IS_U_WHOLE_NUM] = 0;
            nums_type_flags[INDEX_IS_REAL_NUM] = 1;

            if (is_digit(buffer[i]))
            {
                curr_state = F_NUM_SUFFIXES_STATE;
            }
            else if (buffer[i] == '-')
            {
                curr_state = S_EXPONENT_STATE;
            }
            else if (buffer[i] == '+')
            {
                curr_state = S_EXPONENT_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;

                reset_type_count();
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case S_EXPONENT_STATE:
            // printf("Current state: S_EXPONENT_STATE.\t\tCurrent char: %c\n", buffer[i]);
            if (is_digit(buffer[i]))
            {
                curr_state = F_NUM_SUFFIXES_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
                reset_type_count();
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case F_NUM_SUFFIXES_STATE:
            // printf("Current state: F_NUM_SUFFIX_STATE.\t\tCurrent char: %c\n", buffer[i]);
            nums_type_flags[INDEX_IS_OCTAL] = 0;
            nums_type_flags[INDEX_IS_WHOLE_NUM] = 0;
            nums_type_flags[INDEX_IS_U_WHOLE_NUM] = 0;
            nums_type_flags[INDEX_IS_REAL_NUM] = 1;

            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (is_digit(buffer[i]))
            {
                curr_state = F_NUM_SUFFIXES_STATE;
            }
            else if (is_float_suffix(buffer[i]))
            {
                curr_state = F_SUFFIX_STATE;
            }
            else if (is_long_suffix(buffer[i]))
            {
                curr_state = L_D_SUFFIX_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        case L_D_SUFFIX_STATE:
            if (is_delimiter(buffer[i]))
            {
                curr_state = IN_WORD_STATE;
            }
            else if (buffer[i] == END_OF_BUFFER)
            {
                curr_state = END_STATE;
            }
            else
            {
                curr_state = ERROR_STATE;
            }

            i++;

            break;
        default:
            return;
        }
    }
}

// Linux specific
void read_file(int file_d, char **buffer, )
{
 = lseek(file_d, 0, SEEK_END); // Get file size
     == (size_t)-1)
    {
        perror("lseek failed");
        return;
    }

     == 0)
    {
        fprintf(stderr, "File is empty\n");
        return;
    }

    *buffer = mmap(NULL + 1, PROT_READ, MAP_PRIVATE, file_d, 0);

    if (*buffer == MAP_FAILED)
    {
        perror("mmap failed");
        *buffer = NULL;
    }

    return;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *const input_file_name = argv[1];
    const int file_d = open(input_file_name, O_RDONLY);

    if (file_d == -1)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    size_t file_size = 0;
    char *file_content = NULL;

    read_file(file_d, &file_content, &file_size);

    if (!file_content)
    {
        exit(EXIT_FAILURE);
    }

    numbers_identificator(file_content);

    munmap(file_content, file_size);
    close(file_d);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 10
#define POSITIVE_SIGN 0x00
#define NEGATIVE_SIGN 0x0A

// Print BCD (MSB to LSB)
void print_bcd_result(unsigned char bcd[], int size)
{
    printf("HEX: ");
    for (int i = size - 1; i >= 0; i--)
    {
        printf("%02X ", bcd[i]);
    }
    printf("\n");
}

unsigned char bcd_add_single(unsigned char a, unsigned char b, unsigned char *carry_out)
{
    unsigned char result = 0;
    for (int i = 0; i < 2; i++)
    {
        int temp = ((a >> (4 * i)) & 0x0F) +
                   ((b >> (4 * i)) & 0x0F) + *carry_out;

        if (temp > 9)
        {
            temp += 6;
        }
        result |= (temp & 0x0F) << (4 * i);
        *carry_out = (0x10 & temp) >> 4;
    }
    return result;
}

// Convert integer to compressed BCD (2 digits per byte, LSB first)
void int_to_compressed_bcd(int num, unsigned char bcd[], int *size)
{
    unsigned char digits[MAX_DIGITS];
    int digit_count = 0;
    int abs_val = (num < 0) ? -num : num;
    *size = 0;

    do
    {
        digits[digit_count++] = abs_val % 10;
        abs_val /= 10;
    } while (abs_val > 0);

    for (int i = 0; i < digit_count; i += 2)
    {
        unsigned char low = digits[i];
        unsigned char high = (i + 1 < digit_count) ? digits[i + 1] : 0;
        bcd[(*size)++] = (high << 4) | low;
    }
}

void bcd_to_tens_complement(unsigned char bcd[], int size)
{
    unsigned char nine_bcd[MAX_DIGITS];
    memset(nine_bcd, 0x99, size);

    // Subtract bcd from 999...9
    for (int i = 0; i < size; i++)
    {
        unsigned char a = nine_bcd[i];
        unsigned char b = bcd[i];
        unsigned char al = a & 0x0F;
        unsigned char ah = (a >> 4) & 0x0F;
        unsigned char bl = b & 0x0F;
        unsigned char bh = (b >> 4) & 0x0F;

        // Low nibble subtraction with borrow
        if (al < bl)
        {
            al += 10;
            ah -= 1;
        }
        unsigned char res_low = al - bl;

        // High nibble subtraction with borrow
        if (ah < bh)
        {
            ah += 10;
        }
        unsigned char res_high = ah - bh;
        bcd[i] = (res_high << 4) | res_low;
    }

    // Add in BCD
    unsigned char carry = 0;
    bcd[0] = bcd_add_single(bcd[0], 1, &carry);

    for (int i = 1; i < size && carry; i++)
    {
        bcd[i] = bcd_add_single(bcd[i], 0, &carry);
    }
}

void bcd_add(int a, int b, unsigned char result[], int *result_size)
{
    unsigned char bcd_a[MAX_DIGITS] = {0};
    unsigned char bcd_b[MAX_DIGITS] = {0};
    int abs_a = (a < 0) ? -a : a;
    int abs_b = (b < 0) ? -b : b;
    int size_a = 0;
    int size_b = 0;

    int_to_compressed_bcd(abs_a, bcd_a, &size_a);
    int_to_compressed_bcd(abs_b, bcd_b, &size_b);

    // Normalize size
    int max_size = (size_a > size_b) ? size_a : size_b;
    memset(bcd_a + size_a, 0, max_size - size_a);
    memset(bcd_b + size_b, 0, max_size - size_b);

    unsigned char carry = 0;
    int is_negative = 0;

    if ((a >= 0 && b >= 0) || (a < 0 && b < 0)) // Same signs, simple BCD addition
    {
        for (int i = 0; i < max_size; i++)
        {
            result[i] = bcd_add_single(bcd_a[i], bcd_b[i], &carry);
        }

        if (carry)
        {
            result[max_size++] = 1;
        }
        is_negative = (a < 0 && b < 0) ? 1 : 0;
    }
    else
    {
        if (a < 0)
        {
            bcd_to_tens_complement(bcd_a, max_size);
            is_negative = (abs_a > abs_b) ? 1 : 0;
        }
        else if (b < 0)
        {
            bcd_to_tens_complement(bcd_b, max_size);
            is_negative = (abs_b > abs_a) ? 1 : 0;
        }

        for (int i = 0; i < max_size; i++)
        {
            result[i] = bcd_add_single(bcd_a[i], bcd_b[i], &carry);
        }

        if (!carry) // If no carry result is negative, back from 10's complement
        {
            bcd_to_tens_complement(result, max_size);
        }
    }

    // Logic to have only one nible for the sign if possible
    if (result[max_size - 1] > 9) // Indicates that both nibles in the byte are with numbers (no leading zero)
    {
        // Assign the sign to the next byte
        result[max_size] = (is_negative ? NEGATIVE_SIGN : POSITIVE_SIGN);
        *result_size = max_size + 1;
    }
    else
    {
        // Assign the sign to the high nible of the MSB
        result[max_size - 1] = (is_negative ? 0xA0 : POSITIVE_SIGN) + result[max_size - 1];
        *result_size = max_size;
    }
}

unsigned char bcd_multiply_table[10][10];

void init_bcd_multiply_table()
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            int product = i * j;
            unsigned char bcd = ((product / 10) << 4) | (product % 10);
            bcd_multiply_table[i][j] = bcd;
        }
    }
}

void bcd_multiply(int a, int b, unsigned char result[], int *result_size)
{
    init_bcd_multiply_table();

    unsigned char bcd_a[MAX_DIGITS] = {0};
    unsigned char bcd_b[MAX_DIGITS] = {0};
    int size_a = 0;
    int size_b = 0;

    int_to_compressed_bcd(((a < 0) ? -a : a), bcd_a, &size_a);
    int_to_compressed_bcd(((b < 0) ? -b : b), bcd_b, &size_b);

    // Normalize size
    int max_size = (size_a > size_b) ? size_a : size_b;
    memset(bcd_a + size_a, 0, max_size - size_a);
    memset(bcd_b + size_b, 0, max_size - size_b);

    unsigned char temp_digits[4 * MAX_DIGITS + 1] = {0};

    for (int i = 0; i < max_size; i++)
    {
        unsigned char a_low = bcd_a[i] & 0x0F;
        unsigned char a_high = (bcd_a[i] >> 4) & 0x0F;

        for (int j = 0; j < max_size; j++)
        {
            unsigned char b_low = bcd_b[j] & 0x0F;
            unsigned char b_high = (bcd_b[j] >> 4) & 0x0F;

            int a_pos_low = i * 2;
            int a_pos_high = a_pos_low + 1;
            int b_pos_low = j * 2;
            int b_pos_high = b_pos_low + 1;

            // a_low * b_low
            unsigned char product = bcd_multiply_table[a_low][b_low];
            int sum_pos = a_pos_low + b_pos_low;
            temp_digits[sum_pos] += product & 0x0F;
            temp_digits[sum_pos + 1] += (product >> 4) & 0x0F;

            // a_low * b_high
            product = bcd_multiply_table[a_low][b_high];
            sum_pos = a_pos_low + b_pos_high;
            temp_digits[sum_pos] += product & 0x0F;
            temp_digits[sum_pos + 1] += (product >> 4) & 0x0F;

            // a_high * b_low
            product = bcd_multiply_table[a_high][b_low];
            sum_pos = a_pos_high + b_pos_low;
            temp_digits[sum_pos] += product & 0x0F;
            temp_digits[sum_pos + 1] += (product >> 4) & 0x0F;

            // a_high * b_high
            product = bcd_multiply_table[a_high][b_high];
            sum_pos = a_pos_high + b_pos_high;
            temp_digits[sum_pos] += product & 0x0F;
            temp_digits[sum_pos + 1] += (product >> 4) & 0x0F;
        }
    }

    // Process carries
    unsigned char carry = 0;
    for (int i = 0; i < 4 * MAX_DIGITS + 1; i++)
    {
        temp_digits[i] += carry;
        carry = temp_digits[i] / 10;
        temp_digits[i] %= 10;
    }

    // Find highest non-zero digit
    int highest_pos = 0;
    for (int i = 4 * MAX_DIGITS; i >= 0; i--)
    {
        if (temp_digits[i] != 0)
        {
            highest_pos = i;
            break;
        }
    }

    if (highest_pos == 0 && temp_digits[0] == 0)
    {
        highest_pos = 0;
    }

    int num_digits = highest_pos + 1;
    int result_bytes = (num_digits + 1) / 2;

    // Clear result array
    memset(result, 0, MAX_DIGITS);

    for (int i = 0; i < result_bytes; i++)
    {
        int low_pos = 2 * i;
        int high_pos = 2 * i + 1;

        unsigned char low_digit = (low_pos < num_digits) ? temp_digits[low_pos] : 0;
        unsigned char high_digit = (high_pos < num_digits) ? temp_digits[high_pos] : 0;

        result[i] = (high_digit << 4) | low_digit;
    }

    int is_negative = (a < 0) ^ (b < 0);

    if (result_bytes == 0)
    {
        result[0] = POSITIVE_SIGN;
        *result_size = 1;
    }
    else
    {
        if (result[result_bytes - 1] > 9)
        {
            if (result_bytes < MAX_DIGITS)
            {
                result[result_bytes] = is_negative ? NEGATIVE_SIGN : POSITIVE_SIGN;
                *result_size = result_bytes + 1;
            }
            else
            {
                *result_size = result_bytes;
            }
        }
        else
        {
            unsigned char sign_nibble = is_negative ? 0xA0 : 0x00;
            result[result_bytes - 1] = sign_nibble | (result[result_bytes - 1] & 0x0F);
            *result_size = result_bytes;
        }
    }
}

int bcd_compare(int a, int b)
{
    unsigned char bcd_a[MAX_DIGITS] = {0};
    unsigned char bcd_b[MAX_DIGITS] = {0};
    int size_a = 0;
    int size_b = 0;

    int_to_compressed_bcd(((a < 0) ? -a : a), bcd_a, &size_a);
    int_to_compressed_bcd(((b < 0) ? -b : b), bcd_b, &size_b);

    // Normalize size
    int max_size = (size_a > size_b) ? size_a : size_b;
    memset(bcd_a + size_a, 0, max_size - size_a);
    memset(bcd_b + size_b, 0, max_size - size_b);

    // Compare from most significant byte down to LSB
    for (int i = max_size - 1; i >= 0; i--)
    {
        unsigned char byte_a = bcd_a[i];
        unsigned char byte_b = bcd_b[i];

        int high_a = (byte_a >> 4) & 0x0F;
        int high_b = (byte_b >> 4) & 0x0F;
        
        if (high_a != high_b)
        {
            return (high_a > high_b) ? 1 : -1;
        }

        int low_a = byte_a & 0x0F;
        int low_b = byte_b & 0x0F;
        
        if (low_a != low_b)
        {
            return (low_a > low_b) ? 1 : -1;
        }
    }

    // 1 - 0 = 1 -> a > b
    // 0 - 1 = -1 -> a < b
    // 0 - 0 = 0 -> a == b
    return (a > b) - (a < b);
}

int main()
{
    unsigned char result[MAX_DIGITS];
    int result_size;
    int choice = 0;
    int op1 = 0;
    int op2 = 0;

    while (1)
    {
        printf("\n1: Въвеждане на операнд 1\n");
        printf("2: Въвеждане на операнд 2\n");
        printf("3: Сумиране (BCD)\n");
        printf("4: Изваждане (BCD)\n");
        printf("5: Умножение (BCD, до 4 цифри)\n");
        printf("6: Сравнение (BCD, до 4 цифри)\n");
        printf("7: Край\n");
        printf("Избор: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Въведи операнд 1: ");
            scanf("%d", &op1);
            break;

        case 2:
            printf("Въведи операнд 2: ");
            scanf("%d", &op2);
            break;

        case 3:
            bcd_add(op1, op2, result, &result_size);
            printf("Сума (BCD): ");
            print_bcd_result(result, result_size);
            break;

        case 4:
            bcd_add(op1, op2, result, &result_size);
            printf("Разлика (BCD): ");
            print_bcd_result(result, result_size);
            break;

        case 5:
            if (abs(op1) > 9999 || abs(op2) > 9999)
            {
                printf("Умножението е ограничено до 4-цифрени числа!\n");
            }
            else
            {
                bcd_multiply(op1, op2, result, &result_size);
                printf("Произведение (BCD): ");
                print_bcd_result(result, result_size);
            }
            break;

        case 6:
            if (abs(op1) > 9999 || abs(op2) > 9999)
            {
                printf("Сравнението е ограничено до 4-цифрени числа!\n");
            }
            else
            {
                int cmp_result = bcd_compare(op1, op2);
                if (cmp_result > 0)
                    printf("Операнд 1 (%d) е по-голям.\n", op1);
                else if (cmp_result < 0)
                    printf("Операнд 2 (%d) е по-голям.\n", op2);
                else
                    printf("Операндите са равни: %d\n", op1);
            }
            break;

        case 7:
            printf("Край на програмата.\n");
            return 0;

        default:
            printf("Невалиден избор!\n");
            break;
        }
    }

    return 0;
}
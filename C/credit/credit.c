#include <cs50.h>
#include <math.h>
#include <stdio.h>

// get card number from user input
long get_card_number()
{
    long card_number;
    do
    {
        card_number = get_long("Card number: ");
    }
    while (card_number < 1);
    return card_number;
}

// get length of integer
int int_len(long num)
{
    return floor(log10(num)) + 1;
}

// check if value is in an array
int value_in_arr(int value, int array[], int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        if (value == array[i])
        {
            return 1;
        }
    }
    return 0;
}

// truncate number to n digits
int truncate(long num, int digits)
{
    long trunc_numb = num;
    while (int_len(trunc_numb) > digits)
    {
        trunc_numb /= 10;
    }
    return trunc_numb;
}

// validate checksum of cardnumber
int validate_checksum(long card_number)
{
    int first_digit = truncate(card_number, 1);
    int first_two_digits = truncate(card_number, 2);
    int card_length = int_len(card_number);

    // initialize values for checksum algorithm
    int loop_count = 1;
    int checksum = 0;

    // loop through all digits of card_number and apply checksum algorithm
    while (card_number)
    {
        bool is_second_digit = loop_count % 2 == 0;
        int last_digit = card_number % 10;

        // get every second digit from the end, double it, then add the sum of its digits to checksum
        if (is_second_digit)
        {
            int digit = last_digit * 2;
            while (digit)
            {
                checksum += (digit % 10);
                digit /= 10;
            }
        }

        // add all other digits to checksum
        else
        {
            checksum += last_digit;
        }

        card_number /= 10; // shift decimal to remove last digit of card_number
        loop_count ++;
    }

    bool is_valid = checksum % 10 == 0;
    return is_valid;
}

//check card issuer
const char *check_issuer(long card_number)
{
    int first_digit = truncate(card_number, 1);
    int first_two_digits = truncate(card_number, 2);
    int card_length = int_len(card_number);

    int amex[] = {34, 37};
    int mastc[] = {51, 52, 53, 54, 55};

    int amex_arr_size = 2;
    int mastc_arr_size = 5;

    if (first_digit == 4 && (card_length == 13 || card_length == 16))
    {
        return "VISA";
    }

    else if (value_in_arr(first_two_digits, amex, amex_arr_size) && card_length == 15)
    {
        return "AMEX";
    }

    else if (value_in_arr(first_two_digits, mastc, mastc_arr_size) && card_length == 16)
    {
        return "MASTERCARD";
    }

    else
    {
        return "INVALID";
    }
}

int main(void)
{
    long card_number = get_card_number();

    bool is_valid = validate_checksum(card_number);
    if (!is_valid)
    {
        printf("INVALID\n");
        return 0;
    }

    const char *card_issuer = check_issuer(card_number);
    printf("%s\n", card_issuer);
}
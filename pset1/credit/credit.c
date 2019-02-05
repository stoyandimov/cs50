// Validate credit card numbers
// and displays card company
#include <stdio.h>
#include <cs50.h>
#include <string.h>

string get_provider(long number);
bool is_valid(long number);

string amex = "AMEX";
string mastercard = "MASTERCARD";
string visa = "VISA";
string invalid = "INVALID";

int main(int argc, string argv[])
{
    long number;
    if (argc > 1)
    {
        memcpy(&number, argv[1], sizeof(long));
    }
    else
    {
        number = get_long("Number: ");
    }
    printf("%s\n", argv[1]);
    printf("%i\n", argc);
    printf("%li\n", number);

    // Determine card provider
    string provider = get_provider(number);
    // If valid provider, validate checksum
    // printf("%li\n", number);
    if (provider != invalid)
    {
        if (!is_valid(number))
        {
            provider = invalid;
        }
    }

    // Show response
    printf("%s\n", provider);
}

// Constaints
// Name        Length    Starts /w
// AMEX        15        34, 37
// MASTERCARD  16        most 51-55
// VISA        13, 16    4
// Get the
string get_provider(long number)
{
    // Amex check
    if ((number >= 340000000000000 && number <= 349999999999999)
        || (number >= 370000000000000 && number <= 379999999999999))
    {
     return amex;
    }

    // Visa check
    if ((number >= 4000000000000 && number <= 4999999999999)
        || (number >= 4000000000000000 && number <= 4999999999999999))
    {
        return visa;
    }

    // Master Card check
    // Most (BUT NOT ALL) beginnings are in the range of 51-55
    if (number >= 1000000000000000 && number <= 9999999999999999)
    {
        return mastercard;
    }

    // If execution reached next line, it's invalid number
    return invalid;
}

// Validates with Luhn’s Algorithm
bool is_valid(long number)
{
    // Convert long to char[]
    char char_number[16];
    sprintf(char_number, "%li", number);
    // reverse the order
    for (int i = 0; i < 8; i++)
    {
        char char_low = char_number[i];
        char_number[i] = char_number[15-i];
        char_number[15-i] = char_low;
    }
    // Add numbers
    int multiplied_digits_sum = 0;
    int not_multiplied_digits_sum = 0;
    for (int i = 0; i < 16; i++)
    {
        int digit = char_number[i] - '0';

        // (i starts at 0)
        if ((i + 1) % 2 == 0)
        {
            digit *= 2;
            if (digit > 9)
            {
                // Convert int to char[]
                char char_digit[2];
                sprintf(char_digit, "%d", digit);
                // Sum the chars
                digit = char_digit[0] - '0' + char_digit[1] - '0';
            }
            multiplied_digits_sum += digit;
        }
        else
        {
            not_multiplied_digits_sum += digit;
        }

    }
    int checksum = multiplied_digits_sum + not_multiplied_digits_sum;

    // Kratno na 10?
    return checksum % 10 == 0;
}

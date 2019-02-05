// Ceasar CIPHER implementation
// Problem Set 2.1

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

const int MAX_CIPHER = 'z' - 'a' + 1;
const string USAGE_MSG = "Usage: ./caesar key\n";

int is_alphabet(char c);
int is_upper(char c);

int main(int argc, string argv[])
{
    // Parse k from cli args
    if (argc != 2)
    {
        printf("%s", USAGE_MSG);
        return 1;
    }
    string str_k = argv[1];
    for (int i = 0; i < strlen(str_k); i++)
    {
        if (isdigit((int)str_k[i]) == 0)
        {
            printf("%s", USAGE_MSG);
            return 1;
        }
    }

    int k = atoi(argv[1]);
    // Validate k range (must be between a-z or A-Z range)
    if (k <= 0)
    {
        printf("%s", USAGE_MSG);
        return 1;
    }
    if (k >= MAX_CIPHER)
    {
        k %= 26;
    }

    // Get plaintext input that is not empty string
    string plain_text;
    int text_len;
    do
    {
        plain_text = get_string("plaintext: ");
        text_len = strlen(plain_text);
    }
    while (text_len == 0);

    // For each character a-Z char display ciphertext
    printf("ciphertext: ");
    for (int i = 0; i < text_len; i++)
    {
        char c = plain_text[i];
        if (is_alphabet(c))
        {
            if (is_upper(c))
            {
                if (c + k > 'Z')
                {
                    printf("%c", c + k - 'Z' + 'A' - 1);
                }
                else
                {
                    printf("%c", c + k);
                }
            }
            else
            {
                if (c + k > 'z')
                {
                    printf("%c", c + k - 'z' + 'a' - 1);
                }
                else
                {
                    printf("%c", c + k);
                }
            }

        }
        else
        {
            printf("%c", c);
        }
    }
    printf("\n");
}

// Returns 1 if char is between a-z or A-Z
int is_alphabet(char c)
{
    if (c - 'a' < MAX_CIPHER && 'z' - c < MAX_CIPHER)
    {
        return 1;
    }
    if (c - 'A' < MAX_CIPHER && 'Z' - c < MAX_CIPHER)
    {
        return 1;
    }
    return 0;
}

// Returns 1 if character is an upper case letter A-Z
int is_upper(char c)
{
    if (c - 'A' < MAX_CIPHER && 'Z' - c < MAX_CIPHER)
    {
        return 1;
    }
    return 0;
}

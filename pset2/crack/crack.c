// Crack hashed passwords via brute force
// Problem Set 2

#define _XOPEN_SOURCE
#include <unistd.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Hash inputed by user
string hash_input = NULL;
char salt[2] = "  ";

// Check if guess's output match the input hash
int check(const string guess);

int main(int argc, string argv[])
{
    //printf("%s\n",crypt("LOL", "aa"));
    //return 0;
    // Your program should accept one and only one
    // command-line argument: a hashed password.
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    hash_input = argv[1];
    salt[0] = hash_input[0];
    salt[1] = hash_input[1];

    // Assume that each password is composed entirely of alphabetical characters (uppercase/lowercase).
    const string ab = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";

    // Assume that each password has been hashed with C’s DES-based (not MD5-based) crypt function.
    // Program must crack the given password ideally as quickly as possible.
    // Assume that password is no longer than five (5) characters.
    int max_length = 5;
    // LEN 1
    for (int l1 = 0; l1 < strlen(ab); l1++)
    {
        char guess[1] = { ab[l1] };
        if (check(guess) == 0)
        {
            printf("%s\n", guess);
            return 0;
        };
    }
    // LEN 2
    for (int l1 = 0; l1 < strlen(ab); l1++)
    {
        for (int l2 = 0; l2 < strlen(ab); l2++)
        {
            char guess[2] = {ab[l1], ab[l2]};
            if (check(guess) == 0)
            {
                printf("%s\n", guess);
                return 0;
            };
        }
    }
    // LEN 3
    for (int l1 = 0; l1 < strlen(ab); l1++)
    {
        for (int l2 = 0; l2 < strlen(ab); l2++)
        {
            for (int l3 = 0; l3 < strlen(ab); l3++)
            {
                char guess[3] = {ab[l1], ab[l2], ab[l3]};
                if (check(guess) == 0)
                {
                    printf("%s\n", guess);
                    return 0;
                };
            }
        }
    }
    // LEN 4
    for (int l1 = 0; l1 < strlen(ab); l1++)
    {
        for (int l2 = 0; l2 < strlen(ab); l2++)
        {
            for (int l3 = 0; l3 < strlen(ab); l3++)
            {
                for (int l4 = 0; l4 < strlen(ab); l4++)
                {
                    char guess[4] = {ab[l1], ab[l2], ab[l3], ab[l4]};
                    if (check(guess) == 0)
                    {
                        printf("%s\n", guess);
                        return 0;
                    };
                }
            }
        }
    }
    // LEN 5
    for (int l1 = 0; l1 < strlen(ab); l1++)
    {
        for (int l2 = 0; l2 < strlen(ab); l2++)
        {
            for (int l3 = 0; l3 < strlen(ab); l3++)
            {
                for (int l4 = 0; l4 < strlen(ab); l4++)
                {
                    for (int l5 = 0; l5 < strlen(ab); l5++)
                    {
                        char guess[5] = {ab[l1], ab[l2], ab[l3], ab[l4], ab[l5]};
                        if (check(guess) == 0)
                        {
                            printf("%s\n", guess);
                            return 0;
                        };
                    }
                }
            }
        }
    }
}

// Check if guess's output match the input hash
int check(const string guess)
{
    const string hash_output = crypt(guess, salt);
    return strcmp(hash_output, hash_input);
}

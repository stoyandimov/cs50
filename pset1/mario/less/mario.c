#include <cs50.h>
#include <stdio.h>

void print_repeat(char c, int count);

int main(void)
{
    // Prompt for height
    // and validate that height is between 1 and 8
    int h;
    do
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);

    // Drow pyramid
    for (int i = 1; i <= h; i++)
    {
        int empty_count = h - i;
        int block_count = h - empty_count;
        print_repeat(' ', empty_count);
        print_repeat('#', block_count);
        printf("\n");
    }
}
void print_repeat(char c, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("%c", c);
    }
}

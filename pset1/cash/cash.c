// Implemetation of the greedy algorithm
// Problem Set 1

#include <stdio.h>
#include <math.h>
#include <cs50.h>

int main(void)
{
    // Get input in dollars
    // Prompt until a correct float is entered
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);
    
    // Convert dollars to cents
    int cents = round(dollars * 100);
    
    // Devide cents to each coin and set
    // the result as coins and the remainer as cents
    int coins = 0;
    coins += cents / 25;
    cents %= 25;
    coins += cents / 10;
    cents %= 10;
    coins += cents / 5;
    cents %= 5;
    coins += cents;
    
    // Print number of coins
    printf("%i\n", coins);
}

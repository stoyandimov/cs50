# Implemetation of the greedy algorithm
# Problem Set 1
from cs50 import get_float

# Get input in dollars
# Prompt until a correct float is entered
while True:
    cash = get_float("Change owed: ")
    if cash > 0:
        break

# Convert dollars to cents
cents = cash * 100

# Devide cents to each coin and set
# the result as coins and the remainer as cents
coins = 0
coins += int(cents / 25)
cents %= 25
coins += int(cents / 10)
cents %= 10
coins += int(cents / 5)
cents %= 5
coins += cents

# Print the number of coins
print(int(coins))

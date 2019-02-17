# Prints a mario pyramid
from cs50 import get_int


# Repeats a string n times
def repeat(s, n):
    for i in range(n):
        print(s, end="")


# Get height input
while True:
    h = get_int("Height: ")
    if h > 0 and h < 9:
        break


# Loops h times to print pyramid
for x in range(h):
    repeat(" ", h - x - 1)
    repeat("#", x + 1)
    repeat(" ", 2)
    repeat("#", x + 1)
    print()

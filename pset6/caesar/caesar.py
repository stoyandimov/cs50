# Ceasar CIPHER implementation
# Problem Set 2.1

from sys import argv
from cs50 import get_string


# Ensure input contains only k shift
if len(argv) is not 2:
    print("Usage: python caesar.py k")
    exit(1)

# Ensures k is between 0 and 26
k = int(argv[1])
if k > 26:
    k %= 26

# Prompt for plaintext input
plaintext = get_string("plaintext:  ")

# Print ciphertext
print('ciphertext: ', end="")
for c in plaintext:
    if c.isalpha():
        if c.isupper():
            if (ord(c) + k) > ord('Z'):
                print(chr(ord(c) + k - ord('Z') + ord('A') - 1), end="")
            else:
                print(chr(ord(c) + k), end="")
        else:
            if (ord(c) + k) > ord('z'):
                print(chr(ord(c) + k - ord('z') + ord('a') - 1), end="")
            else:
                print(chr(ord(c) + k), end="")
    else:
        print(c, end="")


print()
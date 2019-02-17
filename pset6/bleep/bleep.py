# Censors content
from cs50 import get_string
from sys import argv


def main():
    # Ensure only 1 argument is passed
    if len(argv) is not 2:
        print("Usage: python bleep.py dictionary")
        exit(1)

    # Banned list file
    infile = argv[1]

    # Read banned file into a set
    file = open(infile, "r")
    banned = set()
    for l in file:
        banned.add(l.rstrip("\n"))
    file.close()

    # Parse and censor a message
    msg = get_string("What message would you like to censor?\n")
    for word in msg.split(' '):
        if word.lower() in banned:
            for c in word:
                print("*", end="")
        else:
            print(word, end="")
        print(" ", end="")
    print()


if __name__ == "__main__":
    main()

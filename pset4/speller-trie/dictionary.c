// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

int wordCnt = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];


    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        wordCnt++;
        node *n = root;
        for (int i = 0; word[i] != '\0'; i++)
        {
            int c = word[i] == '\'' ? 26 : tolower(word[i]) - 97;
            if (n->children[c] == NULL)
            {
                n->children[c] = malloc(sizeof(node));
                for (int ii = 0; ii < N; ii++)
                {
                    n->children[c]->children[ii] = NULL;
                }
                n->children[c]->is_word = false;
            }
            n = n->children[c];
        }
        n->is_word = true;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordCnt;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *n = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int c = word[i] == '\'' ? 26 : tolower(word[i]) - 97;
        if (n->children[c] == NULL)
        {
            return false;
        }
        n = n->children[c];
    }
    return n->is_word;
}

void clear(node *n)
{
    for (int i = 0; i < N; i++)
    {
        if (n->children[i] != NULL)
        {
            clear(n->children[i]);
        }
    }
    free(n);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    clear(root);
    return true;
}

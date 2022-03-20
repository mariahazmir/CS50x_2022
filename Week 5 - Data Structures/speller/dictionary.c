// Implements a dictionary's functionality
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 500;

// Hash table
node *table[N];

unsigned int count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    // TODO
    int index = hash(word);
    node *point = table[index];

    while (point != NULL)
    {
        // Find words case insensitively
        if (strcasecmp(point -> word, word) == 0)
        {
            // If words found
            return true;
        }
        // Change pointer
        point = point -> next;
    }

    return false;

}

// Hashes word to a number
unsigned int hash(const char *word)
{

    // TODO: Improve this hash function
    unsigned int n = 0;
    unsigned int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        n = n + 37 * tolower(word[i]);
    }

    n = n % N;
    return n;

}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    // TODO
    // Open dictionary to read
    FILE *file = fopen(dictionary, "r");

    // Handle base case
    if (file == NULL)
    {
        return false;
    }

    char buffer[LENGTH + 1];
    while (fscanf(file, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n -> word, buffer);
        n -> next = NULL;
        int index = hash(buffer);

        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            n -> next = table[index];
            table[index] = n;
        }
        count++;
    }

    fclose(file);
    return true;

}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{

    // TODO
    return count;

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{

    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i] ->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;

}
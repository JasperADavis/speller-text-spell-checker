// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
// Ideal might be 26^3 for ~8 jumps upon collision
const unsigned int N = 26;

// Hash table
node *table[N];

// Track # of words in dict
unsigned int total_words = 0;

// Initialize hash table
void init_hash_table(void)
{
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    // printf("Hash table initialized!\n");
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Run word through hash function
    unsigned int index = hash(word);
    node *current = table[index];

    if (current != NULL)
    {
        while ((current->next != NULL))
        {
            if (strcasecmp(word, current->word) == 0)
            {
                return true;
            }
            current = current->next;
        }
        if (strcasecmp(word, current->word) == 0)
        {
            return true;
        }
    }
    // printf("Line 53 (check function)\n");
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int hash_index = toupper(word[0]) - 'A';
    return hash_index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    init_hash_table();

    // TODO -
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return false;
    }

    // Creates a buffer of sufficient size to store each word
    char buffer[LENGTH];

    // Read each word into the buffer
    while (fscanf(file, "%s", buffer) == 1)
    {
        // Allocate space for new node for each word
        node *new_node = malloc(sizeof(node));
        // Check malloc
        if (new_node == NULL)
        {
            perror("Memory allocation error");
            return false;
        }

        // Run word through hash function
        unsigned int index = hash(buffer);

        // Go to appropriate bin of hash table & create node
        // Link to what's currently at that index (NULL or other node)
        new_node->next = table[index];
        table[index] = new_node;
        strcpy(new_node->word, buffer);
        total_words++;
    }
    // printf("All words saved to hash table!\nTotal words: %i\n", total_words);
    fclose(file);
    return true;
    // return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // printf("Sizing worked.\n\n");
    return total_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            unloader(table[i]);
        }
    }
    // printf("Dictionary nodes unloaded from memory.\n");
    return true;
}

void unloader(node *current)
{
    if (current == NULL)
    {
        return;
    }

    node *next_node = current->next;

    if (next_node != NULL)
    {
        unloader(next_node);
    }
    // Free current node
    free(current);
}

// PREVIOUS IMPLEMENTION OF LOAD
// COUNTED CHARACTERS IN DICTIONARY TO ALLOCATE SIZE FOR ENTIRE DICT AS STRING

// bool load(const char *dictionary)
// {
//     // TODO-C - Function passes a pointer to the name of the dictionary file location
//     // A FILE object is created by opening the file specified
//     FILE *file = fopen(dictionary, "r");
//     if (file == NULL)
//     {
//         perror("Error opening file");
//         return 1;
//     }

//     // Uses fseek to determine total size of dictionary file
//     if (fseek(file, 0, SEEK_END) != 0)
//     {
//         perror("Error seeking to end of dictionary");
//         fclose(file);
//         return 1;
//     }

//     long num_chars = ftell(file);
//     if (num_chars == -1)
//     {
//         perror("Error getting file position");
//         fclose(file);
//         return 1;
//     }

//     // Creates a buffer of sufficient size to store dictionary's contents (+1 for null-terminator)
//     char *buffer = malloc(num_chars + 1);
//     if (buffer == NULL)
//     {
//         perror("Memory allocation error");
//         fclose(file);
//         return 1;
//     }

//     // Move the file pointer back to the beginning of the file
//     if (fseek(file, 0, SEEK_SET) != 0)
//     {
//         perror("Error seeking to beginning of file");
//         free(buffer);
//         fclose(file);
//         return 1;
//     }

//     // Read the file contents into the buffer
//     size_t num_read = fread(buffer, 1, num_chars, file);
//     if (num_read != num_chars)
//     {
//         perror("Error reading file");
//         free(buffer);
//         fclose(file);
//         return 1;
//     }

//     // Null-terminate the buffer
//     buffer[num_chars] = '\0';
//     printf("Dictionary loaded succesfully!\nTotal Number of Characters: %li\n\n", num_chars);
//     return true;

//     // return false;
// }
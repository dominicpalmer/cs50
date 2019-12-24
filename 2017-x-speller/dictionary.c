/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"
#include "hashtable.h"

// make the total words loaded visible
int wordsLoaded;

// make the hashtable visible
node* hashtable[SIZE];

// Loads dictionary into memory. Returns true if successful else false.
bool load(const char* dictionary)
{
    // open the dictionary file
    FILE* dict_ptr = fopen(dictionary, "r");

    // ensure dictionary opens correctly
    if (dict_ptr == NULL)
    {
        return false;
    }

    // create buffer storage for each word
    char wordBuffer[LENGTH + 1];

    // iterate over each character except the null
    while (fscanf(dict_ptr, "%s\n", wordBuffer) != EOF)
    {
        // hash the word to be inputted
        int hashIndex = hashfunction(wordBuffer);

        // allocate memory for the new node
        node* addedNode = malloc(sizeof(node));

        // store word in the new node
        strcpy(addedNode->word, wordBuffer);

        // if hashtable storage at the hash value is free, add the word to this storage space
        if (hashtable[hashIndex] == NULL)
        {
            // set the root pointer to point to the node
            hashtable[hashIndex] = addedNode;

            // set the new node to point to null
            addedNode->next = NULL;
        }

        // if hashtable is occupied at that index, add node to start of its linked list
        else
        {
            // set the new nodes next pointer to the first element of the linked list
            addedNode->next = hashtable[hashIndex];

            // redefine the root of that linked list as the new nodes pointer
            hashtable[hashIndex] = addedNode;
        }

        // increment words loaded by 1 once word is loaded
        wordsLoaded += 1;
    }

    // close file and return success
    fclose(dict_ptr);
    return true;
}


// Returns true if word is in dictionary else false.
bool check(const char* word)
{
    // send word to lowercase and end with null
    int l = strlen(word);
    char wordLower[LENGTH + 1];
    for (int i = 0; i < l; i++)
    {
        wordLower[i] = tolower(word[i]);
    }
    wordLower[l] = '\0';

    // hash the lowercase word
    int hashIndex = hashfunction(wordLower);

    // check if hashtable is empty at that hash value
    if (hashtable[hashIndex] == NULL)
    {
        return false;
    }

    // create cursor to navigate the hashtable
    node* cursor = hashtable[hashIndex];

    // if hashtable[hashIndex] is not null, check all nodes at that index for a match
    while (cursor != NULL)
    {
        if (strcmp(wordLower, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    // return false if no match found
    return false;
}


// Unloads dictionary from memory. Returns true if successful else false.
bool unload()
{
    // iterate through each index of the hashtable
    for (int hashIndex = 0; hashIndex < SIZE; hashIndex++)
    {
        // if the hashtable is not empty at the index, free its linked list
        if (hashtable[hashIndex] != NULL)
        {


            // delete all nodes in the linked list
            while (hashtable[hashIndex] != NULL)
            {
                // create cursor for freeing the linked list
                node* cursor = hashtable[hashIndex];

                // redefine the root one node in
                hashtable[hashIndex] = cursor->next;

                // delete node
                free(cursor);
            }
        }
    }

    // success
    return true;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded.
unsigned int size()
{
    if (wordsLoaded > 0)
    {
        return wordsLoaded;
    }
    else
    {
        return 0;
    }
}

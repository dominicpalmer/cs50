#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "trie.h"
#include "dictionary.h"


// creates and returns a pointer to a node, whose default pointers are to NULL
node* create_node(char character)
{
    // declare a node pointer node to point to null
    node* node = NULL;

    // allocate memory for the node
    node = malloc(sizeof(*node));

    // initialise node with character given
    node->character = character;
    node->next = NULL;
    node->previous = NULL;
    node->parent = NULL;
    node->child = NULL;

    return node;
}


// creates trie by creating root node with null value
void create_trie(node** root)
{
    *root = create_node('\0');
}


// adds word to trie
void trie_add(node** root, char* character)
{
    node* trie_pointer = NULL;

    // declare a temporary pointer used to access the trie, initialised to first non null node in trie
    trie_pointer = (*root)->child;

    // if trie is empty, create first child node
    if (trie_pointer == NULL)
    {
        for (trie_pointer = *root; *character; trie_pointer = trie_pointer->child)
        {
            // create first child node from root
            trie_pointer->child = create_node(*character);

            // set the new child nodes parent pointer to point to the root
            trie_pointer->child->parent = trie_pointer;

            // increment character in word
            character++;
        }

        // create null node after first to signify end of branch
        trie_pointer->child = create_node('\0');

        // set the null node to point back to the first non null node
        trie_pointer->child->parent = trie_pointer;

        return;
    }

    // navigate the trie_pointer as far downwards as the word allows
    while (*character != '\0')
    {
        // check if character of word is equal to what the trie_pointer is pointing to
        if (*character == trie_pointer->character)
        {
            // increment the word to get the next character
            character++;

            // set the trie pointer to start where the character was found
            trie_pointer = trie_pointer->child;
        }
        else
        {
            // once the list cant be followed any further, break from this loop
            break;
        }
    }

    // we are now as far down the trie as we can get for the word

    // navigate the trie_pointer from the last set location as far right as possible
    while (trie_pointer->next)
    {
        // check if the right hand node is our character
        if (*character == trie_pointer->next->character)
        {
            // if it is found, increment the character count
            character++;

            // call the add function for the remainder of the word, starting from this new location as a root
            trie_add(&(trie_pointer->next), character);

            // return once the previous call is complete
            return;
        }

        // increment pointer right and continue
        trie_pointer = trie_pointer->next;
    }

    if (character)
    {
        trie_pointer->next = create_node(*character);
    }
    else
    {
        trie_pointer->next = create_node(*character);
    }

    trie_pointer->next->parent = trie_pointer->parent;
    trie_pointer->next->previous = trie_pointer;

    if (!(*character))
    {
        return;
    }

    character++;

    for(trie_pointer = trie_pointer->next; *character; trie_pointer = trie_pointer->child)
    {
        trie_pointer->child = create_node(*character);
        trie_pointer->child->parent = trie_pointer;
        character++;
    }

    trie_pointer->child = create_node('\0');
    trie_pointer->child->parent = trie_pointer;

    return;
}

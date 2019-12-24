#include "dictionary.h"

#define SIZE 750000


// hashtable node structure
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
} node;

// hashfunction courtesy of djb2
int hashfunction(const char* word);
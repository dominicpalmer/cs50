// header for trie functionality

#include <stdbool.h>

// trie node structure
typedef struct trie_node
{
    char character;
    struct trie_node* next;
    struct trie_node* previous;
    struct trie_node* parent;
    struct trie_node* child;
} node;

// creates a trie data structure
void create_trie(node** root);

// creates root node for a trie
node* create_node(char character);

// adds word to trie
void trie_add(node** root, char* character);

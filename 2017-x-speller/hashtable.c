#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"
#include "hashtable.h"

// hashfunction
int hashfunction(const char* word)
{
    int x;
    int hashAddress = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (isalpha(word[i]))
        {
            x = 34 + word[i] - 'a';
        }
        else
        {
            x = 365;
        }

        hashAddress = ((hashAddress << 3) + x) % SIZE;
    }

    return hashAddress;
}
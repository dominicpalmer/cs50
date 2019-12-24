/**
 * Prompts user for as many as MAX values until EOF is reached,
 * then proceeds to search that array of values for given number.
 *
 * Usage: ./find number
 *
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

// maximum amount of values to go in v_array
const int MAX = 65536;

int main(int argc, string argv[])
{
    // ensure only 1 command-line argument
    if (argc != 2)
    {
        printf("Usage: ./find needle\n");
        return -1;
    }

    // remember number
    int number = atoi(argv[1]);

    // fill array of values
    int size;
    int v_array[MAX];
    for (size = 0; size < MAX; size++)
    {
        // wait for hay until EOF
        printf("\nhaystack[%i] = ", size);
        int new_value = get_int();
        if (new_value == INT_MAX)
        {
            break;
        }

        // add value to v_array
        v_array[size] = new_value;
    }
    printf("\n");

    // sort v_array
    sort(v_array, size);

    // search fo number in v_array
    if (search(number, v_array, size))
    {
        printf("\nFound needle in haystack!\n\n");
        return 0;
    }
    else
    {
        printf("\nDidn't find needle in haystack.\n\n");
        return 1;
    }
}

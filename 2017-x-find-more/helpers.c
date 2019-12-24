// helpers.c
// helper functions for Problem Set 3.

#include <cs50.h>
#include "helpers.h"
#include <tgmath.h>

void merge_arrays(int left_array[], int right_array[], int values[], int n);

// returns true if value is in array of n values, else false
bool search(int value, int values[], int n)
{
    if (n <= 0)
    {
        return false;
    }

    int beginning = 0;
    int end = n - 1;
    int midpoint;
    while (beginning <= end)
    {
        midpoint = ceil((beginning + end)/2);
        if (value  == values[midpoint])
        {
            return true;
        }
        else if (value < values[midpoint])
        {
            end = midpoint - 1;
        }
        else
        {
            beginning = midpoint + 1;
        }
    }
    return false;
}

// sorts array of n values in ascending order
void sort(int values[], int n)
{
    if (n <= 1)
    {
        return;
    }

    // set up midpoint and new arrays
    int midpoint = ceil(n/2);
    int left_array[midpoint];
    int right_array[n - midpoint];

    // fill new arrays
    for ( int i = 0; i < n; i++)
    {
       if (i < midpoint)
       {
           left_array[i] = values[i];
       }
       else
       {
           right_array[i - midpoint] = values[i];
       }
    }

    // sort and merge new arrays
    sort(left_array, midpoint);
    sort(right_array, n - midpoint);
    merge_arrays(left_array, right_array, values, n);
}

// merges two given arrays into a larger array
void merge_arrays(int left_array[], int right_array[], int values[], int n)
{
    int midpoint = ceil(n/2);
    int i = 0;
    int j = 0;
    int left_array_size = midpoint;
    int right_array_size = n - midpoint;

    for (int c = 0; c < n; c++)
    {
        if ((left_array[i] <= right_array[j]  || j >= right_array_size) && i < left_array_size)
        {
            values[c] = left_array[i];
            i++;
        }
        else
        {
            values[c] = right_array[j];
            j++;
        }
    }
}
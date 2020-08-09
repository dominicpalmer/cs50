#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "cs50.h"

int card_length;
int first_digit;

int luhn_validator(long long x)
{
    //calculate length of the card number
    card_length = floor(log10(x)) + 1;

    //if length of card number not equal to 13, 15 or 16, print INVALID and exit program, otherwise carry on
    if (card_length != 13 && card_length != 15 && card_length != 16)
    {
        printf("INVALID\n");
        exit(0);
    }

    //send card number digits in reverse to an array
    int card_number_array[16];
    int i = 0;
    while (x != 0)
    {
        card_number_array[i] = x % 10;
        x /= 10;
        i++;
    }

    //store first digit of card_number_array as first_digit
    first_digit = card_number_array[card_length - 1];

    //sum odd elements of card_number_array
    int sum1 = 0;
    for (int n = 0; n < card_length; n+=2)
    {
        sum1 = sum1 + card_number_array[n];
    }

    //sum 2*evenelements of card_number_array
    int sum2 = 0;
    int temp_array[2];
    for (int u = 1; u < card_length; u+=2)
    {
        int j = 2 * card_number_array[u];
        if (j >= 10)
        {
            int k = 0;
            while (j != 0)
            {
                temp_array[k] = j % 10;
                j /= 10;
                k++;
            }
            sum2 = sum2 + temp_array[0] + temp_array[1];
        }
        else
        {
            sum2 = sum2 + j;
        }
    }

    //total sum ready for validation
    int total_sum = sum1 + sum2;

    //check if total_sum mod 10 is 0
    if (total_sum % 10 == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void determine_issuer(int x, int y)
{
    if (x == 3 && y == 15)
    {
        printf("AMEX\n");
    }
    else if (x == 5 && (y == 16))
    {
        printf("MASTERCARD\n");
    }
    else if (x == 4 && (y == 13 || y == 16))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

int main()
{
    //request card number
    long long card_number = 0;
    do
    {
        printf("Number: ");
        card_number = get_long_long();
    } while (card_number < 0);

    //run the luhn_validator, then determine the issuer if required
    if (luhn_validator(card_number) == 1)
    {
        printf("INVALID\n");
        exit(0);
    }
    else
    {
        determine_issuer(first_digit, card_length);
    }

    return 0;
}
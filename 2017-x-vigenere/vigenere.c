#include <stdio.h>
#include "string.h"
#include <cs50.h>
#include <ctype.h>

//function to check if entire string is alpha only
int is_string_alpha(char* str)
{
	int res = 1;
	for (int i = 0, l = strlen(str); i < l; i++)
	{
		if (isalpha(str[i]) == 0)
		{
			res = 0;
		}
	}
	return res;
}

//vigenere encryption
void vigenere_encryptor(char* plaintext, char* keyword)
{
	int p_length = strlen(plaintext);
	int k_length = strlen(keyword);
	char ciphertext[p_length];
	int j = 0;
	printf("ciphertext: ");
	for ( int i = 0; i < p_length; i++ )
	{
		j = j % k_length;
		if (!isalpha(plaintext[i]))
		{
			printf("%c", plaintext[i]);
		}
		else
		{
			if ( islower(plaintext[i]) && islower(keyword[j]) )
			{
				ciphertext[i] = ((( plaintext[i] - 97) + ( keyword[j] - 97)) % 26) + 97;
				printf("%c", ciphertext[i]);
			}
			else if ( islower(plaintext[i]) && isupper(keyword[j]) )
			{
				ciphertext[i] = ((( plaintext[i] - 97) + ( keyword[j] - 65)) % 26) + 97;
				printf("%c", ciphertext[i]);
			}
			else if ( isupper(plaintext[i]) && isupper(keyword[j]) )
			{
				ciphertext[i] = ((( plaintext[i] - 65) + ( keyword[j] - 65)) % 26) + 65;
				printf("%c", ciphertext[i]);
			}
			else if ( isupper(plaintext[i]) && islower(keyword[j]) )
			{
				ciphertext[i] = ((( plaintext[i] - 65) + ( keyword[j] - 97)) % 26) + 65;
				printf("%c", ciphertext[i]);
			}
			j++;
		}
	}
	printf("\n");
}

int main(int argc, char* argv[])
{
	//check theres only 1 command line argument
	if ( argc != 2 )
	{
		printf("Usage: ./vigenere k\n");
		return 1;
	}

	//check the command line argument only contains alphabetical characters
	if ( is_string_alpha(argv[1]) == 0 )
	{
		printf("Usage: ./vigenere k\n");
		return 1;
	}

	//request plaintext from user
	char* plaintext;
	printf("plaintext: ");
	plaintext = get_string();

	//run the encryptor and print the ciphertext
	vigenere_encryptor(plaintext, argv[1]);
	return 0;
}

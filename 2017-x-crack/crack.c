#include <stdio.h>
#include <crypt.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define _XOPEN_SOURCE

const char* salt = "50";
char alphabet[53] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

//function to hash all 1 digit alphabetical characters, and compare to the passed in hash
void cracker1(char* hash)
{
    for (int i = 0; i < 52 ; i++)
    {
        char password[2] = {alphabet[i]};
        char* test_hash = crypt(password, salt);
        if ( strcmp(test_hash, hash) == 0 )
        {
            printf("%s\n", password);
            exit(0);
        }
    }
}

//function to hash all 2 digit alphabetical characters, and compare to the passed in hash
void cracker2(char* hash)
{
    for (int i = 0; i < 52 ; i++)
    {
         for (int j = 0; j < 52 ; j++)
        {
            char password[3] = {alphabet[i], alphabet[j]};
            char* test_hash = crypt(password, salt);
            if ( strcmp(test_hash, hash) == 0 )
            {
                printf("%s\n", password);
                exit(0);
            }
        }
    }
}

//function to hash all 3 digit alphabetical characters, and compare to the passed in hash
void cracker3(char* hash)
{
    for (int i = 0; i < 52 ; i++)
    {
         for (int j = 0; j < 52 ; j++)
        {
            for (int k = 0; k < 52 ; k++)
            {
                char password[4] = {alphabet[i], alphabet[j], alphabet[k]};
                char* test_hash = crypt(password, salt);
                if ( strcmp(test_hash, hash) == 0 )
                {
                    printf("%s\n", password);
                    exit(0);
                }
            }
        }
    }
}

//function to hash all 4 digit alphabetical characters, and compare to the passed in hash
void cracker4(char* hash)
{
    for (int i = 0; i < 52 ; i++)
    {
         for (int j = 0; j < 52 ; j++)
        {
            for (int k = 0; k < 52 ; k++)
            {
                for (int l = 0; l < 52 ; l++)
                {
                    char password[5] = {alphabet[i], alphabet[j], alphabet[k], alphabet[l]};
                    char* test_hash = crypt(password, salt);
                    if ( strcmp(test_hash, hash) == 0 )
                    {
                        printf("%s\n", password);
                        exit(0);
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    //check theres only 1 command line argument
	if ( argc != 2 )
	{
		printf("Usage: ./crack hash");
		return 1;
	}

	//check the command line argument is 13 characters long
	if ( strlen(argv[1]) != 13 )
	{
		printf("Usage: ./crack hash");
		return 1;
	}

    //run the exclusive crackers in turn, exiting when one of them finds the password
    cracker1(argv[1]);
    cracker2(argv[1]);
    cracker3(argv[1]);
    cracker4(argv[1]);

    //if none of the crackers find the password, print result
    printf("Password not found. Was the input a real DES hash?\n");
    return 0;
}

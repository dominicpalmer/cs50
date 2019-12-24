#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main()
{
    //request input of name
    string name = get_string();
    for (int i = 0; i < strlen(name); i++)
    {
        if ((i==0 || name[i - 1] == ' ') && name[i] != ' ')
        {
            printf("%c", toupper(name[i]));
        }
    }
    printf("\n");
    return 0;
}
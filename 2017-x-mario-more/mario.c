#include <stdio.h>
#include <cs50.h>

int main()
{
    int height;
    do
    {
        printf("Height: ");
        height = get_int();
    } while (height < 0 || height > 23);

    for (int i = 0; i <= height; i++)
    {
        int x = height - i;
        if (x != height)
        {
        printf("%.*s", x, "                       ");
        printf("%.*s", i, "#######################");
        printf("  ");
        printf("%.*s", i, "#######################");
        printf("\n");
        }
    }

    return 0;
}

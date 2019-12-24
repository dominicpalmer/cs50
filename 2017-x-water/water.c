#include <cs50.h>
#include <stdio.h>

int main()
{
	printf("How long was your shower in minutes?");
	int minutes;
	do
	{
		minutes = get_int();
	} while (minutes < 0);

	int bottles = minutes*12;
	printf("Minutes: %d\n", minutes);
	printf("Bottles: %d\n", bottles);
	return 0;
}

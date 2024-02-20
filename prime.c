// TDD(Test Driven Development) on prime Number program.

#include <stdio.h>
#include <string.h>
#include <math.h>

int Number = 0, Counter = 0, IsPrime;

int prime();

int MakeInputValid(char TestCase[])
{
	int Number = atoi(TestCase), Length = strlen(TestCase), Count = 0;
	Count = (Number == 0) ? 1 : log10(Number) + 1;
	if(Count == Length && strcmp(TestCase, "0"))
	{
		return Number;
	}
	else
	{
		return -1;
	}
}

int prime(char TestCase[])
{
	Number = MakeInputValid(TestCase);
	if(Number == 2 || Number == 3)
	{
		IsPrime = 1;
	}
	else if(Number == 0)
	{
		IsPrime = 0;
	}
	else if (Number % 2 == 0)
	{
		IsPrime = 0;
	}
	else if (Number = 1)
	{
		for(Counter = 2; Counter * Counter <= Number; Counter++)
		{
			if(Number % Counter == 0)
			{
				IsPrime = 0;
				break;
			}
			else
			{
				IsPrime = 1;
			}
		}
	}
}
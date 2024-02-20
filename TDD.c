// implementation of ttd.

#include <stdio.h>
#include "prime.c"


void main() 
{
	TestPrime();
}
   
int TestPrime()
{
    int Counter, PassCounter = 0;
    char TestCases[20][10] = {"-7", "0", "1", "2", "3", "4", "10", "14", "31", "a", "valli", "7ram", "ra7", "%", " ", "\t", "/", "  "};
    int TestCasesSize = 20;

    for (Counter = 0; Counter < TestCasesSize; Counter++) 
    {
        if (prime(TestCases[Counter]) < 0)
        {
            printf("Failed(%s)\n", TestCases[Counter]);
        }
        else
        {
            PassCounter++;
        }
    }
    if (Counter == PassCounter)
    {
        printf("All tests passed!\n");
    }
    return 0;
}

#include <stdio.h>

void Swap(int* x, int*y)
{
    if (x != y)
    {
        *x = *x ^ *y;
        *y = *y ^ *x;
        *x = *y ^ *x;
    }
}

void SwapChar(char* x, char*y)
{
   if (x != y)
    {
        *x = *x ^ *y;
        *y = *y ^ *x;
        *x = *y ^ *x;
    }

   /* char temp = *y;
    *x = *y;
    *y = temp;*/
}


void PrintArray(int* arr, int size)
{
    printf("\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("\n");
}

int Ceil(int number, int divisor)
{
    return (number + (divisor -1))/divisor;
}

void ResetBit(int* p, int bit)
{
    *p &= ~(1 << bit);
    
}
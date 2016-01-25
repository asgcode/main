#include <stdio.h>

void printSetRange(int x)
{
    int start = 0;
    int end = 0;
    int current  = 0;
    int negative  = 0;
    int range = 0;

    if (x < 0)
    {
        negative = 1;
        x = x & 0x7fffffff;
    }

    printf("\n");

    while (x != 0)
    {

        if (x & 0x1)
        {
            if (start == 0)
            {
                start = current;
                end = current;
            }
            else
            {
                end++;
            }
        }
        else
        {
            if (start != 0)
            {
                printf ("set bit range %d is from %d to %d\n", range, start, end);
                start = 0;
                end = 0;
                range++;
            }
        }
        current++;
        x = x >> 1;
    }

    if (start != 0)
    {
        printf ("set bit range %d is from %d to %d\n", range, start, end);
    }

    printf("\n");
}
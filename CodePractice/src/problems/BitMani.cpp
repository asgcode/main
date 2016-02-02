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

int GiveOnceOnlyNumber(int* pArray, int size)
{
    int once = 0;
    int twice = 0;
    int common_bit = 0;
    for (int i = 0; i < size; i++)
    {
        // if number is coming third time, twice would still keep that number in it.
        twice = twice | (once & pArray[i]);
        once = once ^ pArray[i];

        // add twice happening number back to once.
        // if there is nothing in twice we are good with below statement
        // but after twice. when the number would come third time.
        // So overall problem is find a way to nullify below statement when this number is happening the third time.
        // clean up number from once without effecting it for the third time. 
        // or clean up twice after the second instance.
        // once = once ^ twice;

        // clean up twice after the second instance.
        // In below statement twice would essentially became same as once.
        // twice = twice ^ pArray[i];
        // how about?
        // twice = 0;

        // Or other strategy is basically remove number which are there in both once and twice from once.
        common_bit = once & twice;

        //remove common bit from once;
        once = once & ~(common_bit);

    }

    return once;
}
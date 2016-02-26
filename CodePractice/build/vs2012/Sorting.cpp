#include <Utils.h>

int PerformShellSort(int* pArray, unsigned int size)
{
    // Perform h-sort
    int h, i, k;

    for (h = size/2; h >= 1; h = h/2)
    {
        for (i = h; i < size; i += h)
        {
            k = i;
            // Perform insertion of k by swapping until its lesser than its left neighbour
            while (((k - h) >= 0) && (pArray[k] < pArray[k - h]))
            {
                Swap(&pArray[k], &pArray[k-h]);
                k = k - h;
            }
        }
    }

    return 0;
}
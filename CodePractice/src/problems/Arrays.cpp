#include "Utils.h"

int FindNElementRepeatNBy2Times(int* pArray, int size)
{
    // record 2 element total, ceil(size/2), ceil(size/2) + 1
    // if they are equal return any one of them if not return ceil(size/2) -1
    int middle = ceil(size, 2) - 1;

    if (size <= 2)
    {
        // return either one
        return pArray[0];
    }

    if (pArray[middle] == pArray[middle + 1])
    {
        return pArray[middle];
    }
    else
    {
        return pArray[middle - 1];
    }


}
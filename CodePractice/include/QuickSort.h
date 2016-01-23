#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "Utils.h"

int partition(int* arr, int low, int high)
{
    int pivot = arr[high];
    int k     = low;

    // iterate from low to high such that all elements
    // lower than pivot stays to left and higher than pivot
    // goes to the right
    for (int i = low; i < high; i++)
    {
        if (arr[i] < pivot)
        {
            swap(&arr[i], &arr[k]);
            k++;
        }
    }

    // bring pivot to its spot by performing a swap operation
    swap(&arr[k], &arr[high]);

    return k;
}

void QuickSort(int* arr, int low, int high)
{
    if (high <= low)
    {
        return;
    }

    int pivot = partition(arr, low, high);

    QuickSort(arr, low, pivot-1);
    QuickSort(arr, pivot+1, high);
}

#endif
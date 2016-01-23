#include <iostream>
using namespace std;
//{12, 11, 13, 5, 6, 7};



void swap(int* a, int* b)
{
    if(*a != *b)
    {
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}


//O(logn)

void heapify(int* A, int ix, int size)
{
    int max = ix;
    
    int leftChildix = (ix<<1)+1;
    int rightChildix = leftChildix+1; // 2x+2 2(x+1)
    
    if( (leftChildix < size) && (A[leftChildix]>A[ix]))
    {
        max = leftChildix;
    }
    
    if( (rightChildix < size) && (A[rightChildix] >A[max]) )
    {
        max = rightChildix;
    }
    
    if(max != ix)
    {
        swap(&A[ix], &A[max]);
        heapify(A, max, size);
    }
    
}

void CreateHeap(int* A , int size)
{
    //O(nlogn)
    for(int i = ((size/2)-1); i >=  0 ; i--)
    {
        heapify(A, i, size);
    }
    
    
    
}

void printArray(int* A, int size)
{
    printf("\r\n");
    for(int i = 0 ; i < size; i++)
    {
        printf("%d ", A[i]);
    }
    printf("\r\n");
}


void heapSort(int* A, int size)
{
    CreateHeap(A, size);
    printArray(A, size);
    while(size>0)
    {
        swap(&A[0], &A[size-1]);
        size--;
        heapify(A, 0, size);
    }
}



int main() {
	//code
	int A[] = {12, 11, 13, 5, 6, 7, 15};
	int size = sizeof(A)/sizeof(A[0]);
	
	printArray(A, size);
	
    heapSort(A, size);
	
	printArray(A, size);
	return 0;
}
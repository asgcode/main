#include <stdio.h>

int merge(int* dest, int* source, int low, int mid, int high)
{
    int i = low;
    int j = mid+1;
    int k = low;
    
    while ((i <= mid) && (j <= high))
    {
        if (source[i] < source[j])
        {
            dest[k++] = source[i++];
        }
        else
        {
            dest[k++] = source[j++];
        }
    }
    
    while (i <= mid)
    {
        dest[k++] = source[i++];
    }
    
    while (j <= high)
    {
        dest[k++] = source[j++];
    }
    
    k--;
    
    if (k == high)
    {
        printf("high is same as k\n");
    }
    
    while(k >= low)
    {
        source[k] = dest[k];
        k--;
    }
    
    
    return 0;
}

int mergeSort(int* dest, int* source, int low, int high)
{
    int mid = low + (high -low)/2;
    
    if (high > low)
    {
        mergeSort(dest, source, low, mid);
        mergeSort(dest, source, mid+1, high);
        
        merge(dest, source, low, mid, high);
    }
    
    return 0;
}

int main() {
    
	int source[] = {3, 1, -1,  5, 2, 2, 4};
	int dest[] = {3, 1, 5, -1,  2, 2, 4};
	
	mergeSort(dest, source, 0, ((sizeof(source)/sizeof(int)) - 1));
	
	printf("\nsorted array is ");
	
	for(int i = 0; i < (sizeof(source)/sizeof(int)); i++)
	{
	    printf("%d\t", dest[i]);
	}
	
	printf("\n");
	
	return 0;
}
#include <assert.h>

void heapify(unsigned short* pHeap, int i)
{
    int rootIndex  = i;

}
/// TODO Revise Method Name
void BuildMinHeap(unsigned short* pHeap, int numEntries)
{
    // Create heap of first 32 entries
    assert(numEntries == 48);

    for (int i = (numEntries/2 -1); i >= 0; i++)
    {
        heapify(pHeap, i);
    }


}

///TODO make sure short shouldn't be unsigned short
void CopyIn16BitHeap(short* pHeap, unsigned char* pData, int heapSizeInBytes)
{
#if 0
    int i = 0;

     // Create heap of first 32 entries
    assert(heapSizeInBytes == 48);

    while (heapSizeInBytes >= 3)
    {
        // Get 3 bytes on each iteration, covert it 2 16 bit
        pHeap[heapLoc] =  ((pData[i] << 4) | ((pData[i+1] & (0xF << 4)) >> 4));
        pHeap[heapLoc + 1] = ((pData[i+1] & 0xF) << 8) | pData[i+2];

        heapSizeInBytes -= 3;
        i += 3;
        heapLoc += 2;
    }
#endif
}

/// TODO Revise Method Name
void InsertIn12BitHeap(int* pHeap, int heapSizeInBytes, int* pInputBuffer, int inSize)
{
    // Create heap of first 32 entries
    assert(inSize >= 32);
    assert(heapSizeInBytes == 48);


}

void print12BitHeap(int* pHeap, int NumOfHeadDWords)
{
}


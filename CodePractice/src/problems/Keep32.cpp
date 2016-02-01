#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/// Dont make assumption on input file size.It can be bigger than the avaliable memory on the
/// device. So we just want to read only configurable buffer size data from the file.
/// Multiple fRead can slow down your performance so best would to keep the numbers of fRead
/// as small as possible by keeping the size of MinEntriesSizeInBytes as big as feasible.
enum
{
    MinEntriesSizeInBytes = 48
};

enum
{
    NumOfQueueEntries = 32
};

typedef unsigned char UCHAR;

typedef struct _CircularBuffer
{
    short* pData;
    int    head;
    int    tail;
}CircularBuffer;



void SwapShort(short* x, short*y)
{
    if (x != y)
    {
        *x = *x ^ *y;
        *y = *x ^ *y;
        *x = *x ^ *y;
    }
}

inline short get12BitFrom3Bytes(UCHAR* pByte, int position)
{
    int i = 0;

    if (position == 0)
    {
        // Return first 12 bit
        return ((pByte[i] << 4) | ((pByte[i+1] & (0xF << 4)) >> 4));
    }
    else if (position == 1)
    {
        // Return second 12 bit
        return (((pByte[i+1] & 0xF) << 8) | pByte[i+2]);
    }
    else
    {
        assert(!"Unexpected position value");
        return -1;
    }
}

void PerformSort(CircularBuffer* pCBuf, int numEntries)
{
    for (int i = 0; i < numEntries; i++)
    {
        for (int j = i; j > 0; j--)
        {
            if (pCBuf->pData[j] < pCBuf->pData[j-1])
            {
                SwapShort(&pCBuf->pData[j], &pCBuf->pData[j-1]); 
            }
            else
            {
                break;
            }
        }
    }

    pCBuf->head = numEntries - 1;
    pCBuf->tail = 0;
}

void InsertElement(CircularBuffer* pCBuf, short num)
{
    if (num <= pCBuf->pData[pCBuf->tail])
    {
        return;
    }
    else if (num >= pCBuf->pData[pCBuf->head])
    {
        pCBuf->head = (pCBuf->head + 1)% NumOfQueueEntries;
        pCBuf->tail = (pCBuf->tail + 1)% NumOfQueueEntries;
        pCBuf->pData[pCBuf->head] = num;
    }
    else
    {
        int i = ((pCBuf->tail + 1) % NumOfQueueEntries);

        // insert number into the queue
        while (i != pCBuf->head)
        {
            if (num > pCBuf->pData[i])
            {
                continue;
            }
            else
            {
                int j = i;
                // Insert the element at i and move all the elements to right
                while (j != ((pCBuf->tail+1) % NumOfQueueEntries))
                {
                    int temp = pCBuf->pData[j];
                    pCBuf->pData[j] = num;
                    num = temp;
                    j = ((j + 1)% NumOfQueueEntries);
                }
                break;
            }

            i = (i + 1)% NumOfQueueEntries;
        }
    }
}

void InsertEntries(
                   CircularBuffer* pCBuf,
                   UCHAR* pInData,
                   int sizeOfEntriesInBytes)
{
    int i = 0;

    while (sizeOfEntriesInBytes > 1)
    {
          // Get 3 bytes on each iteration, covert it 2 16 bit
        InsertElement(pCBuf, get12BitFrom3Bytes(&pInData[i], 0));
        if (sizeOfEntriesInBytes > 2)
        {
            InsertElement(pCBuf, get12BitFrom3Bytes(&pInData[i], 1));
        }
        else
        {
            break;
        }
        i += 3;
        sizeOfEntriesInBytes -= 3;
    }

}


void CopyIn16BitCirQueue(
                         CircularBuffer* pCBuf,
                         UCHAR* pInData,
                         int dataSizeInBytes)
{
    int i        = 0;
    int nextAvailIndex = 0;

    assert(dataSizeInBytes <= MinEntriesSizeInBytes);

    // There must be atleast two byte in input stream
    while (dataSizeInBytes > 1)
    {
        // Get 3 bytes on each iteration, covert it 2 16 bit
         pCBuf->pData[nextAvailIndex] = get12BitFrom3Bytes(&pInData[i], 0);

        if (dataSizeInBytes > 2)
        {
            pCBuf->pData[nextAvailIndex + 1] = get12BitFrom3Bytes(&pInData[i], 1);
        }
        else
        {
            nextAvailIndex += 1;
            break;
        }

        dataSizeInBytes -= 3;
        i += 3;
        nextAvailIndex += 2;
    }

    // Next avaliable index is same number of entries in circular buffer
    PerformSort(pCBuf, nextAvailIndex);

}

void WriteOutput(
                 const char* pFileName,   ///TODO parameter comments
                 UCHAR * pInData,
                 int inStart,
                 int inputSizeInBytes,
                 CircularBuffer* pCBuf)
{
    FILE* hInFile = NULL;
    int   i = 0;

    /// TODO Perform Null check
    hInFile = fopen(pFileName, "wb");

    fputs("--Sorted Max 32 Values--\r\n", hInFile);

    ///TODO: Figure if duplicates, then 32 means 32 + duplicates?
    for (i = pCBuf->tail;; i = ((i + 1) % NumOfQueueEntries))
    {
        fprintf(hInFile, "%d \r\n", pCBuf->pData[i]);

        if (i == pCBuf->head)
        {
            break;
        }
    }

    fputs("--Last 32 Values--\r\n", hInFile);

    // Start from the given index in the input array
    i = inStart;

    while (inputSizeInBytes > 1)
    {
        // Get 3 bytes on each iteration, covert it 2 12 bit and print it out.
        fprintf(hInFile, "%d \r\n", get12BitFrom3Bytes(&pInData[i], 0));

        if (inputSizeInBytes > 2)
        {
            fprintf(hInFile, "%d \r\n", get12BitFrom3Bytes(&pInData[i], 1));
        }
        else
        {
            break;
        }

        inputSizeInBytes = inputSizeInBytes - 3;
        i += 3;

        ///TODO Think of a better name for index i and try to do something more elegent than just resetting 
        if (i >= MinEntriesSizeInBytes)
        {
            i = 0;
        }
    }

    ///TODO when inputSizeInBytes have non multiple of 3 bytes of entries.

    fclose(hInFile);

}

int PrintBest32(const char* pInFilePath, const char* pOutFilePath)
{
    //Read the input from a file in configurable size of buffer
    /// TODO move inReadBuffer & heap from stack to dynamically allocated to avoid stack overflow.
    UCHAR*         pReadBuffer         = NULL;
    CircularBuffer CirBuffer           = {0};
    FILE*          hInFile             = NULL;
    int            readSizeInBytes     = 0;
    long           fileSizeInBytes     = 0;
    int            readBufferStartByte = 0;
    int            isCirBufSorted      = false;
    long           outSizeInBytes      = 0;


    /// TODO perform NULL check
    hInFile = fopen(pInFilePath, "rb");

    // obtain file size
    fseek(hInFile, 0, SEEK_END);
    fileSizeInBytes = ftell(hInFile);
    rewind(hInFile);

    readSizeInBytes = (fileSizeInBytes > MinEntriesSizeInBytes) ? MinEntriesSizeInBytes : fileSizeInBytes;

    if (readSizeInBytes > 1)
    {
        int numOf12BitEntries = ((readSizeInBytes * 8)/12);
        CirBuffer.pData  = (short*) malloc (sizeof(short) * numOf12BitEntries);
        if (CirBuffer.pData == NULL)
        {
            ///TODO CirBuffer.pData NULL check
            return -1;
        }
        pReadBuffer = (UCHAR*) malloc (sizeof(UCHAR) * readSizeInBytes);
        if (pReadBuffer == NULL)
        {
            ///TODO pReadBuffer NULL check
            return -1;
        }

        outSizeInBytes = readSizeInBytes;
    }

    while (fileSizeInBytes > 1)
    {
        ///TODO check the implication of reading data in bytes
        ///TODO check the assumption of size of char equal to bytes
        int retSize = fread(pReadBuffer, sizeof(UCHAR), readSizeInBytes, hInFile);

        assert(retSize == readSizeInBytes);

        if (isCirBufSorted == false)
        {
            ///TODO align discrepency between char and unsigned char
            CopyIn16BitCirQueue(&CirBuffer, pReadBuffer, readSizeInBytes);
            isCirBufSorted = true;
        }
        else
        {
            InsertEntries(&CirBuffer, pReadBuffer, readSizeInBytes);
        }

        fileSizeInBytes -= readSizeInBytes;

        if (fileSizeInBytes > 1)
        {
            readSizeInBytes = (fileSizeInBytes > MinEntriesSizeInBytes) ? MinEntriesSizeInBytes : fileSizeInBytes;
            if (readSizeInBytes < MinEntriesSizeInBytes)
            {
                readBufferStartByte = readSizeInBytes;
            }
        }
    }

    if (outSizeInBytes > 1)
    {
        WriteOutput(pOutFilePath, pReadBuffer, readBufferStartByte, outSizeInBytes, &CirBuffer);
    }

    fclose(hInFile);

    ///TODO return proper error type.
    return 0;
}
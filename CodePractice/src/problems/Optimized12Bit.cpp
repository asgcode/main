#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/// Dont make assumption on input file size.It can be bigger than the avaliable memory on the
/// device. So we just want to read only configurable buffer size data from the file.
/// Multiple fRead can slow down your performance so best would to keep the numbers of fRead
/// as small as possible by keeping the size of MinEntriesSizeInBytes as big as feasible.

enum
{
    NumOfQueueEntries = 32
}
enum
{
    NumBitsIn12BitEntries = 12
}
enum
{
    NumBitsInByte = 8
}
};

typedef unsigned char UCHAR;

typedef struct _CircularBuffer
{
    UCHAR* pData;
    int    head;
    int    tail;
}CircularBuffer;


inline short get12BitFrom3Bytes(
                                UCHAR* pByte,
                                int position)
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

inline short GetValue(
                      UCHAR* pByte,  /// Byte pointer
                      int position)  /// 0 based position value
{
    int index = ((position) * 12)/8;
    if (position % 2 == 0)
    {
       // Odd numbers are at first position of byte
        return ((pByte[index] << 4) | ((pByte[index + 1] & (0xF << 4)) >> 4));
    }
    else
    {
         //Even numbers are at second half of byte
        return (((pByte[index] & 0xF) << 8) | pByte[index + 1]);
    }
}

inline void SetValue(
                      UCHAR* pByte,  /// Byte pointer
                      int position,  /// 0 based position value
                      short num)

{
    int index = ((position) * 12)/8;
    if (position % 2 == 0)
    {
        // Odd numbers's right most nibble would go in
        // consequtive byte's left nibble.
        pByte[index] = (num & 0xFF0) >> 4;
        pByte[index + 1] = (pByte[index + 1] & 0x0F) | ((num & 0xF) << 4);
        
    }
    else
    {
        // Even numbers's left most nibble is at second nibble of first byte
        // and remaining byte at complete next index
        pByte[index] = (pByte[index] & 0xF0) | ((num & 0xF00) >> 8);
        pByte[index + 1] = num & 0xFF;
    }

}

void PerformSort(
                 CircularBuffer* pCBuf,
                 int numEntries)
{
    for (int i = 0; i < numEntries; i++)
    {
        for (int j = i; j > 0; j--)
        {
            short current  = GetValue(pCBuf->pData, j);
            short previous = GetValue(pCBuf->pData, j-1);
            if (current < previous)
            {
                // Swap 12 bit numbers
                SetValue(pCBuf->pData, j, previous);
                SetValue(pCBuf->pData, (j - 1), current);
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

void InsertElement(
                   CircularBuffer* pCBuf,
                   short num)
{
    if (num <= GetValue(pCBuf->pData, pCBuf->tail))
    {
        return;
    }
    else if (num >= GetValue(pCBuf->pData, pCBuf->head))
    {
        pCBuf->head = (pCBuf->head + 1)% NumOfQueueEntries;
        pCBuf->tail = (pCBuf->tail + 1)% NumOfQueueEntries;
        SetValue(pCBuf->pData, pCBuf->head, num);
    }
    else
    {
        int i = ((pCBuf->tail + 1) % NumOfQueueEntries);

        // insert number into the queue
        while (i != pCBuf->head)
        {
            if (num > GetValue(pCBuf->pData, i))
            {
                continue;
            }
            else
            {
                int j = i;
                // Insert the element at i and move all the elements to right
                while (j != ((pCBuf->tail+1) % NumOfQueueEntries))
                {
                    short temp = GetValue(pCBuf->pData, j);
                    SetValue(pCBuf->pData, j, num);
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


void WriteOutput(
                 const char* pFileName,   ///TODO parameter comments
                 UCHAR * pInData,
                 int inStart,
                 int inputSizeInBytes,
                 CircularBuffer* pCBuf)
{
    FILE* hInFile               = NULL;
    int   i                     = 0;
    int   minEntriesSizeInBytes = (NumOfQueueEntries * NumBitsIn12BitEntries) / NumBitsInByte;

    /// TODO Perform Null check
    hInFile = fopen(pFileName, "wb");

    fputs("--Sorted Max 32 Values--\r\n", hInFile);

    ///TODO: Figure if duplicates, then 32 means 32 + duplicates?
    for (i = pCBuf->tail;; i = ((i + 1) % NumOfQueueEntries))
    {
        fprintf(hInFile, "%d \r\n", GetValue(pCBuf->pData, i));

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
        if (i >= minEntriesSizeInBytes)
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
    UCHAR*         pReadBuffer           = NULL;
    CircularBuffer CBuffer               = {0};
    FILE*          hInFile               = NULL;
    int            readSizeInBytes       = 0;
    long           fileSizeInBytes       = 0;
    int            readBufferStartByte   = 0;
    int            isCirBufSorted        = false;
    long           outSizeInBytes        = 0;
    int            minEntriesSizeInBytes = (NumOfQueueEntries * NumBitsIn12BitEntries) / NumBitsInByte;


    /// TODO perform NULL check
    hInFile = fopen(pInFilePath, "rb");

    // obtain file size
    fseek(hInFile, 0, SEEK_END);
    fileSizeInBytes = ftell(hInFile);
    rewind(hInFile);

    readSizeInBytes = (fileSizeInBytes > MinEntriesSizeInBytes) ? MinEntriesSizeInBytes : fileSizeInBytes;

    if (readSizeInBytes > 1)
    {
        CBuffer.pData  = (UCHAR*) calloc (readSizeInBytes, sizeof(UCHAR));
        if (CBuffer.pData == NULL)
        {
            ///TODO CBuffer.pData NULL check
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
            memcpy(CBuffer.pData, pReadBuffer, readSizeInBytes);
            PerformSort(&CBuffer, ((readSizeInBytes * 8)/12));
            isCirBufSorted = true;
        }
        else
        {
            InsertEntries(&CBuffer, pReadBuffer, readSizeInBytes);
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
        WriteOutput(pOutFilePath, pReadBuffer, readBufferStartByte, outSizeInBytes, &CBuffer);
    }

    fclose(hInFile);

    ///TODO return proper error type.
    return 0;
}
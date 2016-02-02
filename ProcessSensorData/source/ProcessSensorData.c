/** @file ProcessSensorData.c
 *  @brief Entry routine for processing the sensor data
 *
 *  This file process the input data to retrieve max NumOfBufEntries
 *  and keep track of last NumOfBufEntries.
 *
 */

#include "FileIO.h"
#include "BitUtils.h"
#include "Utils.h"

static void PerformSort(CircularBuffer* pCBuf)
{
    for (int i = 0; i < pCBuf->numEntries; i++)
    {
        for (int j = i; j > 0; j--)
        {
            short current  = Get12BitEntry(pCBuf->pData, j);
            short previous = Get12BitEntry(pCBuf->pData, j-1);

            if (current < previous)
            {
                // Swap 12 bit numbers
                Set12BitEntry(pCBuf->pData, j, previous);
                Set12BitEntry(pCBuf->pData, (j - 1), current);
            }
            else
            {
                break;
            }
        }
    }

    pCBuf->head = pCBuf->numEntries - 1;
    pCBuf->tail = 0;

}

static INT InsertEnryToSortedBuf(CircularBuffer* pCBuf, short num)
{
    if (pCBuf->numEntries != MaxNumOfBufEntries)
    {
        // Hard requirement to calculate correct circular buffer index 
        return -1;
    }

    if (num <= Get12BitEntry(pCBuf->pData, pCBuf->tail))
    {
        return;
    }
    else if (num >= Get12BitEntry(pCBuf->pData, pCBuf->head))
    {
        pCBuf->head = (pCBuf->head + 1)% pCBuf->numEntries;
        pCBuf->tail = (pCBuf->tail + 1)% pCBuf->numEntries;
        Set12BitEntry(pCBuf->pData, pCBuf->head, num);
    }
    else
    {
        int i = ((pCBuf->tail + 1) % pCBuf->numEntries);

        // insert number into the queue
        while (i != pCBuf->head)
        {
            if (num > Get12BitEntry(pCBuf->pData, i))
            {
                continue;
            }
            else
            {
                int j = i;

                // Insert the element at i and move all the elements to right
                while (j != ((pCBuf->tail+1) % pCBuf->numEntries))
                {
                    short temp = Get12BitEntry(pCBuf->pData, j);
                    Set12BitEntry(pCBuf->pData, j, num);
                    num = temp;
                    j = ((j + 1)% pCBuf->numEntries);
                }
                break;
            }

            i = (i + 1)% pCBuf->numEntries;
        }
    }

    return -1;
}


INT ProcessSensorData(UCHAR*          pReadData,
                      UINT            sizeInBytes,
                      CircularBuffer* pProcessedCBuf)   /// Pointer to processed output data
{
    UINT numEntries = (sizeInBytes * NumBitsPerByte)/NumBitsPerEntries;

    if ((numEntries < 1) || (pReadData == NULL) || (pProcessedCBuf == NULL))
    {
        return -1;
    }


    if (pProcessedCBuf->pData == NULL)
    {
        pProcessedCBuf->pData  = (UCHAR*) calloc (sizeInBytes, sizeof(UCHAR));
        if (pProcessedCBuf->pData == NULL)
        {
            ///TODO CBuffer.pData NULL check
            return -1;
        }
        pProcessedCBuf->numEntries = numEntries;
        memcpy(pProcessedCBuf->pData, pReadData, sizeInBytes);
        PerformSort(pProcessedCBuf);
    }
    else
    {
        UINT readEntryNum = 0;

        while (numEntries > 0)
        {
            InsertEnryToSortedBuf(pProcessedCBuf, Get12BitEntry(pReadData, readEntryNum));
            readEntryNum++;
            numEntries--;
        }
    }
    return 0;
}


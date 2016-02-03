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
#include "Knobs.h"

static VOID insertAndShiftRightCBuf(CircularBuffer* pCBuf, USHORT num, UINT pos)
{
    // Insert the element at i and move all the elements to right
    UINT entriesInCBuf = pCBuf->numEntries;

    while (j != ((pCBuf->tail+1) % MaxNumOfBufEntries))
    {
        short temp = Get12BitEntry(pCBuf->pData, j);
        Set12BitEntry(pCBuf->pData, j, num);
        num = temp;
        j = ((j + 1)% MaxNumOfBufEntries);
    }
}

static VOID PerformSort(CircularBuffer* pCBuf)
{
    UINT i, j;

    for (i = 0; i < pCBuf->numEntries; i++)
    {
        for (j = i; j > 0; j--)
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
    //if (pCBuf->numEntries != MaxNumOfBufEntries)
    //{
    //    // Hard requirement to calculate correct circular buffer index 
    //    return -1;
    //}

    if (pCBuf->numEntries == MaxNumOfBufEntries)
    {
        if (num <= Get12BitEntry(pCBuf->pData, pCBuf->tail))
        {
            return 0;
        }
        else if (num >= Get12BitEntry(pCBuf->pData, pCBuf->head))
        {
            pCBuf->head = (pCBuf->head + 1)% MaxNumOfBufEntries;
            pCBuf->tail = (pCBuf->tail + 1)% MaxNumOfBufEntries;
            Set12BitEntry(pCBuf->pData, pCBuf->head, num);
        }
        else
        {


            int i = ((pCBuf->tail + 1) % MaxNumOfBufEntries);

            // insert number into the queue
            while (i != pCBuf->head)
            {
                if (num <= Get12BitEntry(pCBuf->pData, i))
                {
                    int j = i;

                    // Insert the element at i and move all the elements to right
                    while (j != ((pCBuf->tail+1) % MaxNumOfBufEntries))
                    {
                        short temp = Get12BitEntry(pCBuf->pData, j);
                        Set12BitEntry(pCBuf->pData, j, num);
                        num = temp;
                        j = ((j + 1)% MaxNumOfBufEntries);
                    }

                    pCBuf->head = (pCBuf->head + 1)% MaxNumOfBufEntries;
                    pCBuf->tail = (pCBuf->tail + 1)% MaxNumOfBufEntries;

                    break;
                }

                i = (i + 1)% MaxNumOfBufEntries;
            }
        }
    }
    else
    {
        if (pCBuf->numEntries == 0)
        {
            Set12BitEntry(pCBuf->pData, 0, num);
        }
        else if (num >= Get12BitEntry(pCBuf->pData, pCBuf->head))
        {
            pCBuf->head++;
            Set12BitEntry(pCBuf->pData, pCBuf->head, num);
        }
        else
        {
            // find first entry from tail to head bigger than num
            // and insert right before it and shift numbers to right
            // by one and increase the head count
            UINT i = pCBuf->tail;

            while (i <= pCBuf->head)
            {
                if (num <= Get12BitEntry(pCBuf->pData, i))
                {
                    UINT j = i;

                    // Insert the element at i and move all the elements to right
                    while (j <= (pCBuf->head + 1))
                    {
                        short temp = Get12BitEntry(pCBuf->pData, j);
                        Set12BitEntry(pCBuf->pData, j, num);
                        num = temp;
                        j++;
                    }

                    pCBuf->head++;
                    break;
                }
                i++;
            }
        }
        pCBuf->numEntries++;
    }

    return 0;
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


    /*if (pProcessedCBuf->numEntries == 0)
    {
        pProcessedCBuf->numEntries = numEntries;
        memcpy(pProcessedCBuf->pData, pReadData, sizeInBytes);
        PerformSort(pProcessedCBuf);
    }
    else*/
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


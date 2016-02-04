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

/*******************************************************************************************************************************
*   insertAndShiftRightCBuf
*
*   @brief
*       insert a number to the circular buffer and shifts all the entries to the right
*
*   @return
*       N/A
*
*******************************************************************************************************************************/
static VOID insertAndShiftRightCBuf(CircularBuffer* pCBuf, ///< Buffer in which number needs to be inserted 
                                    USHORT num,            ///< Number to be inserted
                                    UINT pos)              ///< 0 based index at which number needs to be inserted
{
    UINT entriesInCBuf = pCBuf->numEntries;
    UINT maxAllowedShifts = (MaxNumOfBufEntries > entriesInCBuf) ? entriesInCBuf :  (MaxNumOfBufEntries - 1);

    // Insert at given index and keep moving numbers to right until we hit last entry in the buffer
    while (pos <= maxAllowedShifts)
    {
        USHORT tmp = Get12BitEntry(pCBuf->pData, pos);
        Set12BitEntry(pCBuf->pData, pos, num);
        num = tmp;
        pos = ((pos + 1) % MaxNumOfBufEntries);
        maxAllowedShifts--;
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

                    //// Insert the element at i and move all the elements to right
                    //while (j != ((pCBuf->tail+1) % MaxNumOfBufEntries))
                    //{
                    //    short temp = Get12BitEntry(pCBuf->pData, j);
                    //    Set12BitEntry(pCBuf->pData, j, num);
                    //    num = temp;
                    //    j = ((j + 1)% MaxNumOfBufEntries);
                    //}

                    insertAndShiftRightCBuf(pCBuf, num, j);

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

                    //// Insert the element at i and move all the elements to right
                    //while (j <= (pCBuf->head + 1))
                    //{
                    //    short temp = Get12BitEntry(pCBuf->pData, j);
                    //    Set12BitEntry(pCBuf->pData, j, num);
                    //    num = temp;
                    //    j++;
                    //}

                    insertAndShiftRightCBuf(pCBuf, num, j);

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


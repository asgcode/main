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
    UINT entriesInCBuf    = pCBuf->numEntries;
    UINT maxAllowedShifts = (MaxNumOfBufEntries > entriesInCBuf) ? (entriesInCBuf) :  (MaxNumOfBufEntries - 1);
    UINT startPos         = pos;

    // Insert at given index and keep moving numbers to right until we either hit last entry in the buffer
    // or come back to the same pos where we started.
    do
    {
        USHORT tmp = Get12BitEntry(pCBuf->pData, pos);
        Set12BitEntry(pCBuf->pData, pos, num);
        num = tmp;
        pos = ((pos + 1) % MaxNumOfBufEntries);

    }while ((pos != startPos) && (pos <= maxAllowedShifts));

    // We inserted something so head must always advance
    pCBuf->head = (pCBuf->head + 1)% MaxNumOfBufEntries;
}

static INT InsertEnryToSortedCBuf(CircularBuffer* pCBuf,
                                  short num)
{
    // First number in the queue
    if (pCBuf->numEntries == 0)
    {
        Set12BitEntry(pCBuf->pData, 0, num);
    }
    // number is less than tail
    else if (num <= Get12BitEntry(pCBuf->pData, pCBuf->tail))
    {
        if (pCBuf->numEntries == MaxNumOfBufEntries)
        {
            // We are already full and all our buf entries are
            // bigger than this one so discard it
            return 0;
        }
        else
        {
            // Just insert it at tail
            insertAndShiftRightCBuf(pCBuf, num, pCBuf->tail);
        }
    }
    // Bigger than head just insert at right end
    else if (num >= Get12BitEntry(pCBuf->pData, pCBuf->head))
    {
        pCBuf->head = (pCBuf->head + 1)% MaxNumOfBufEntries;
        Set12BitEntry(pCBuf->pData, pCBuf->head, num);

        if (pCBuf->numEntries == MaxNumOfBufEntries)
        {
            // If queue already full, entry at the tail got overwritten
            // so update the tail
            pCBuf->tail = (pCBuf->tail + 1)% MaxNumOfBufEntries;
        }
    }
    // Less than head and bigger than tail, got to insert in the middle
    else
    {
        // Start with next entry after tail as reaching here means
        // we want to insert number bigger than tail
        int i = ((pCBuf->tail + 1) % MaxNumOfBufEntries);

        // Can't go upto head otherwise number would have been already
        // inserted before.
        while (i != pCBuf->head)
        {
            // Find first entry bigger or same as number
            if (num <= Get12BitEntry(pCBuf->pData, i))
            {
                insertAndShiftRightCBuf(pCBuf, num, i);
                if (pCBuf->numEntries == MaxNumOfBufEntries)
                {
                    // If queue already full, entry at the tail got overwritten
                    // so update the tail
                    pCBuf->tail = (pCBuf->tail + 1)% MaxNumOfBufEntries;
                }
                break;
            }

            i = (i + 1)% MaxNumOfBufEntries;
        }
    }

    // Unless we are full, entry always gets inserted
    if (pCBuf->numEntries < MaxNumOfBufEntries)
    {
        pCBuf->numEntries++;
    }
    return 0;
}


INT ProcessSensorData(CircularBuffer* pReadCBuf,
                      CircularBuffer* pProcessedCBuf)   /// Pointer to processed output data
{
    UINT numEntries   = pReadCBuf->numEntries;
    UINT readEntryNum = 0;

    if (numEntries < 1)
    {
        return -1;
    }

    while (numEntries > 0)
    {
        InsertEnryToSortedCBuf(pProcessedCBuf, Get12BitEntry(pReadCBuf->pData, readEntryNum));
        readEntryNum++;
        numEntries--;
    }
    return 0;
}


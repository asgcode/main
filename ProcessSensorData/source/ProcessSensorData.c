/*******************************************************************************************************************************
 *  @file  ProcessSensorData.c
 *  @brief Provides a public routine for processing the sensor data by first reading the data from a input unordered circular
 *         buffer and copy it to ordered circular buffer. Ordered circular buffer can keep maximum 32, 12 bit unsigned entries.
 *         Once the buffer gets full, it would accept only those entry which are at least bigger than its minimum entry. Thus
 *         providing constant insertion time per entry for mostly sorted data which usually the case with fitness sensors.
 *
 *
 *******************************************************************************************************************************/

#include "FileIO.h"
#include "BitUtils.h"
#include "Utils.h"
#include "Knobs.h"

/*******************************************************************************************************************************
*   InsertByRightShift
*
*   @brief
*       Insert a number to the given ordered circular buffer by shifting all of its right entries by one position to the right.
*       If no more room on right side, entries would overflow to left most position just like a traditional circular buffer
*
*   @return
*       N/A
*
*******************************************************************************************************************************/
static VOID InsertByRightShift(CircularBuffer* pCBuf,      ///< Buffer in which number needs to be inserted 
                               USHORT          entryValue, ///< Value of the entry which needs to be inserted
                               UINT            entryPos)   ///< Position at which the entry needs to be inserted
{
    UINT entriesInCBuf         = pCBuf->numEntries;
    UINT maxAllowedRightShifts = (MaxNumOfBufEntries > entriesInCBuf) ? (entriesInCBuf) :  (MaxNumOfBufEntries - 1);
    UINT nextEntryPos          = entryPos;
    UINT nextEntryVal          = entryValue;

    // Insert at entryPos index and keep moving numbers to right until we either hit last entry in the buffer
    // or come back to the same pos where we started.
    do
    {
        USHORT tmp = Get12BitEntry(pCBuf->pData, nextEntryPos);
        Set12BitEntry(pCBuf->pData, nextEntryPos, nextEntryVal);
        nextEntryVal = tmp;
        nextEntryPos = (nextEntryPos + 1) % MaxNumOfBufEntries;

    }while ((nextEntryPos != entryPos) && (nextEntryPos <= maxAllowedRightShifts));

    // We inserted something so head must always advance
    pCBuf->head = (pCBuf->head + 1) % MaxNumOfBufEntries;
}

/*******************************************************************************************************************************
*   InsertEnryToSortedCBuf
*
*   @brief
*       Insert a entry to the given ordered or sorted circular buffer. Entry would gets inserted only if its bigger than its
*       minimum or left most (in other words entry at tail position) value.
*
*   @return
*       N/A
*
*******************************************************************************************************************************/
static INT InsertEnryToSortedCBuf(CircularBuffer* pCBuf,
                                  USHORT          entryValue)
{
    // First some validity checks
    if ((entryValue & 0xF000) != 0)
    {
        // Invalid entry
        printf("ERRROR: Only 12 bits entries are accepted \r\n");
        return -1;
    }

    // First number in the circular buffer
    if (pCBuf->numEntries == 0)
    {
        Set12BitEntry(pCBuf->pData, 0, entryValue);
    }
    // number is less than tail
    else if (entryValue <= Get12BitEntry(pCBuf->pData, pCBuf->tail))
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
            InsertByRightShift(pCBuf, entryValue, pCBuf->tail);
        }
    }
    // Bigger than head just insert at right end
    else if (entryValue >= Get12BitEntry(pCBuf->pData, pCBuf->head))
    {
        pCBuf->head = (pCBuf->head + 1)% MaxNumOfBufEntries;
        Set12BitEntry(pCBuf->pData, pCBuf->head, entryValue);

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
            if (entryValue <= Get12BitEntry(pCBuf->pData, i))
            {
                InsertByRightShift(pCBuf, entryValue, i);
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


/*******************************************************************************************************************************
 *  @file   ProcessSensorData.c
 *  @brief  Defines a public routine for processing the sensor data by first reading the data from a input unordered circular
 *          buffer and copy it to ordered circular buffer. Ordered circular buffer can keep maximum 32, 12 bit unsigned entries.
 *          Once the buffer gets full, it would accept only those entry which are at least bigger than its minimum entry. Thus
 *          providing constant insertion time per entry for mostly sorted data which usually the case with fitness sensors.
 *  @author Amit Bansal
 *
 *
 *******************************************************************************************************************************/
#include "FileIO.h"
#include "CommonUtils.h"

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
static VOID InsertByRightShift(CircularBuffer* pCBuf,      ///< [in] Buffer in which number needs to be inserted 
                               USHORT          entryValue, ///< [in] Value of the entry which needs to be inserted
                               UINT            entryPos)   ///< [in] Position at which the entry needs to be inserted
{
    UINT entriesInCBuf         = pCBuf->numEntries;
    UINT maxAllowedRightShifts = (MaxNumOfBufEntries > entriesInCBuf) ? (entriesInCBuf) : (MaxNumOfBufEntries - 1);
    UINT nextEntryPos          = entryPos;
    UINT nextEntryVal          = entryValue;

    // Something is going to be inserted so head must always advance
    pCBuf->head = (pCBuf->head + 1) % MaxNumOfBufEntries;


    // Insert at entryPos index and keep moving numbers to right until we either hit last entry in the buffer
    // or until we overwrite the tail with shifted head value
    do
    {
        USHORT tmp = Get12BitEntry(pCBuf->pData, nextEntryPos);
        Set12BitEntry(pCBuf->pData, nextEntryPos, nextEntryVal);
        nextEntryVal = tmp;
        nextEntryPos = (nextEntryPos + 1) % MaxNumOfBufEntries;

    }while (nextEntryPos != ((pCBuf->head + 1) % MaxNumOfBufEntries));

}

/*******************************************************************************************************************************
*   InsertEnryToSortedCBuf
*
*   @brief
*       Insert a entry to the given ordered or sorted circular buffer. Entry would gets inserted only if its bigger than its
*       minimum or left most (in other words entry at tail position) value.
*
*   @return
*       -1 if inserted number is invalid or 0 upon success
*
*******************************************************************************************************************************/
static INT InsertEnryToSortedCBuf(CircularBuffer* pCBuf,         //< [in] Circular buffer in which entry needs to be inserted
                                  USHORT          entryValue)    //< [in] 12 bit value that needs to be inserted
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
                    pCBuf->tail = (pCBuf->tail + 1) % MaxNumOfBufEntries;
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

/*******************************************************************************************************************************
*   ProcessSensorData
*
*   @brief
*       Entry point routine for processing the sensor data. It receives pointer to unordered read circular buffer at the input
*       and outputs a pointer to a ordered or sorted circular buffer. Client is responsible for allocating memory for both the
*       input and output circular buffer. Output sorted circular buffer accepts only those entries from input read buffer which
*       are greater than its least valued entry.
*
*   @return
*       -1 if 
*
*******************************************************************************************************************************/
INT ProcessSensorData(CircularBuffer* pReadCBuf,        /// [in]     Pointer to input unordered circular queue buffer
                      CircularBuffer* pMaxSortedCBuf)   /// [in\out] Pointer to updated sorted max circular buffer
{
    UINT numInputEntries = pReadCBuf->numEntries;
    UINT readEntryNum    = 0;

    if ((numInputEntries < 1) || (pReadCBuf->pData == NULL) || (pMaxSortedCBuf->pData == NULL))
    {
        printf("ERROR: Invalid input to %s \r\n", __FUNCTION__);
        return -1;
    }

    while (readEntryNum < numInputEntries)
    {
        InsertEnryToSortedCBuf(pMaxSortedCBuf, Get12BitEntry(pReadCBuf->pData, readEntryNum));
        readEntryNum++;
    }

    return 0;
}


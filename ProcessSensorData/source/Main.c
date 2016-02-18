/*******************************************************************************************************************************
 *  @file   Main.c
 *  @brief  Defines program entry point and perform following key operations
 *          1) Read input file to a circular unordered queue in a loop, 32 - 12Bit entries at a time until data is finished.
 *          2) Process 32 - 12Bit entries at a time by picking up biggest 32 entries from all of the input data.
 *          3) Write first 32 max entries retrieved in step 2 and next last 32 read entries from the unordered circular buffer
 *             to the given output file.
 *  @author Amit Bansal
 *
 *
 *******************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CommonUtils.h"
#include "FileIO.h"
#include "ProcessSensor.h"

/*******************************************************************************************************************************
*   TestReadSortAndWrite12BitNumbers
*
*   @brief
*       Generic routine to provide first read, sort the input data and write the max 32 and last 32 entries using the given
*       pair of input and output file.
*
*   @return
*       N/A
*
*******************************************************************************************************************************/
BOOL TestReadSortAndWrite12BitNumbers(const char* pInputFileName, const char* pOutputFileName)
{
    UINT  totalNumEntries, maxBufEntries;
    FILE*          hInputFile    = NULL;
    FILE*          hOutputFile   = NULL;
    CircularBuffer cReadBuf      = {0};
    CircularBuffer cProcessedBuf = {0};
    BOOL           status        = TRUE;

    if ((pInputFileName == NULL) || (pOutputFileName == NULL))
    {
        printf("ERROR: Invalid input or output file \r\n");
        status = FALSE;
    }

    if (status == TRUE)
    {
        hInputFile = OpenFile(pInputFileName, "rb");
        if (hInputFile == NULL)
        {
            printf("ERROR: Failed to open input file %s \r\n", pInputFileName);
            status = FALSE;
        }
    }

    if (status == TRUE)
    {
        hOutputFile = OpenFile(pOutputFileName, "wb");
        if (hOutputFile == NULL)
        {
            printf("ERROR: Failed to open output file %s", pOutputFileName);
            status = FALSE;
        }
    }

    if (status == TRUE)
    {
        totalNumEntries = GetFileEntries(hInputFile);
        if (totalNumEntries < 1)
        {
            printf("Error: Insufficient data in input file \r\n");
            status = FALSE;
        }
    }

    if (status == TRUE)
    {
        maxBufEntries = (totalNumEntries > MaxNumOfBufEntries) ? MaxNumOfBufEntries : totalNumEntries;

        cReadBuf.pData = (UCHAR*) malloc(CONVERT_ENTRIES_TO_BYTE(maxBufEntries) * sizeof(UCHAR));
        if (cReadBuf.pData == NULL)
        {
            printf("ERROR: Unable to allocate 12 DWORDs for read circular buffer \r\n");
            status = FALSE;
        }
    }

    if (status == TRUE)
    {
        cProcessedBuf.pData = (UCHAR*) malloc(CONVERT_ENTRIES_TO_BYTE(maxBufEntries) * sizeof(UCHAR));
        if (cProcessedBuf.pData == NULL)
        {
            printf("ERROR: Unable to allocate 12 DWORDs for sorted max circular buffer \r\n");
            status = FALSE;
        }
    }


    if (status == TRUE)
    {
        UINT entriesToBeRead = totalNumEntries;

        while (entriesToBeRead != 0)
        {
            // Read maximum MaxNumOfBufEntries i.e. 32 entries at a time from input into circular buffer
            UINT numEntries, ret;

            numEntries = (entriesToBeRead > MaxNumOfBufEntries) ? MaxNumOfBufEntries : entriesToBeRead;
            ret        = ReadInput(hInputFile, numEntries, &cReadBuf);
            if (ret != 0)
            {
                status = FALSE;
                break;
            }

            ret = ProcessSensorData(&cReadBuf, &cProcessedBuf);
            if (ret != 0)
            {
                status = FALSE;
                break;
            }

            entriesToBeRead -= numEntries;
        }
    }

    if (status == TRUE)
    {
        WriteStringToOutput(hOutputFile, "--Sorted Max 32 Values--\r\n");
        WriteCBufToOutput(hOutputFile, &cProcessedBuf);

        // If we have more than 32 entries then we should
        // all of the last 32 entries in circular buffer and
        // not just the most recently read entries.
        if (totalNumEntries > MaxNumOfBufEntries)
        {
            cReadBuf.numEntries = MaxNumOfBufEntries;
        }

        WriteStringToOutput(hOutputFile, "--Last 32 Values--\r\n");
        // Write out max allowed (i.e. 32) last read entries from circular read buffer
        WriteCBufToOutput(hOutputFile, &cReadBuf);
    }

    // Clean up
    if (hOutputFile != NULL)
    {
        CloseFile(hOutputFile);
    }
    if (hInputFile != NULL)
    {
        CloseFile(hInputFile);
    }

    free(cReadBuf.pData);
    cReadBuf.pData = NULL;

    free(cProcessedBuf.pData);
    cProcessedBuf.pData = NULL;

    return status;
}

/*******************************************************************************************************************************
*   main
*
*   @brief
*       program entry point
*
*   @return
*       N/A
*
*******************************************************************************************************************************/

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage: ProcessSensorData <input file> <output file> e.g. ProcessSensorData test1.bin test1.out \r\n");
    }
    else
    {
        if (0 == TestReadSortAndWrite12BitNumbers(argv[1], argv[2]))
        {
           printf("ERROR: %s execution unsuccessful \r\n", argv[0]); 
        }
        else
        {
           printf("%s execution successful \r\n", argv[0]);
        }
    }

    return 0;
}
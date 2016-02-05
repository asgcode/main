#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BitUtils.h"
#include "Utils.h"
#include "FileIO.h"
#include "Knobs.h"
#include "ProcessSensor.h"


INT TestReadSortAndWrite12BitNumbers(const char* pInputFileName, const char* pOutputFileName)
{
    FILE* hInputFile;
    FILE* hOutputFile;
    UINT  totalNumEntries, maxBufEntries;
    CircularBuffer cReadBuf;
    CircularBuffer cProcessedBuf;
    BOOL status = TRUE;

    hInputFile = OpenFile(pInputFileName, "rb");
    if (hInputFile == NULL)
    {
        printf("ERROR: Failed to open input file %s", __LINE__);
        status = FALSE;
    }

    hOutputFile = OpenFile(pOutputFileName, "wb");
    if (hOutputFile == NULL)
    {
        printf("ERROR: Failed to open output file %s", __LINE__);
        status = FALSE;
    }

    if (status == TRUE)
    {
        memset(&cReadBuf, 0x0, sizeof(CircularBuffer));
        memset(&cProcessedBuf, 0x0, sizeof(CircularBuffer));

        totalNumEntries = GetFileEntries(hInputFile);
        if (totalNumEntries < 1)
        {
            printf("Error: Insufficient data in input file");
            status = FALSE;
        }
    }

    if (status == TRUE)
    {
        maxBufEntries = (totalNumEntries > MaxNumOfBufEntries) ? MaxNumOfBufEntries : totalNumEntries;

        cReadBuf.pData = (UCHAR*) malloc(ConvertEntriesToByte(maxBufEntries) * sizeof(UCHAR));
        if (cReadBuf.pData == NULL)
        {
            printf("ERROR: Out of memory");
            status = FALSE;
        }
        cProcessedBuf.pData = (UCHAR*) malloc(ConvertEntriesToByte(maxBufEntries) * sizeof(UCHAR));
        if (cProcessedBuf.pData == NULL)
        {
            printf("ERROR: Out of memory");
            status = FALSE;
        }
    }


    if (status == TRUE)
    {
        UINT lastReadEntries;
        UINT entriesToBeRead = totalNumEntries;

        while (entriesToBeRead != 0)
        {
            cReadBuf.numEntries = (entriesToBeRead > MaxNumOfBufEntries) ? MaxNumOfBufEntries : entriesToBeRead;
            ReadInput(hInputFile, &cReadBuf);

            ProcessSensorData(&cReadBuf, &cProcessedBuf);
            lastReadEntries = cReadBuf.numEntries;
            entriesToBeRead -= lastReadEntries;
        }

        WriteOutput(hOutputFile, TRUE, &cProcessedBuf);

        // If we have more than 32 entries then move tail
        // to just after the last set of read entries so
        // we print out most recent set of last read entries
        if (totalNumEntries > MaxNumOfBufEntries)
        {
            cReadBuf.tail = lastReadEntries % MaxNumOfBufEntries;
            cReadBuf.numEntries = MaxNumOfBufEntries;
        }

        // Write out max allowed last read entries from circular read buffer
        WriteOutput(hOutputFile, FALSE, &cReadBuf);
    }

    // Clean up
    CloseFile(hOutputFile);
    CloseFile(hInputFile);
    free(cReadBuf.pData);
    free(cProcessedBuf.pData);
    return 0;
}
int main()
{
    TestReadSortAndWrite12BitNumbers("..//..//samples//test1.bin", "..//..//samples//test1.out");
    TestReadSortAndWrite12BitNumbers("..//..//samples//test2.bin", "..//..//samples//test2.out");
    TestReadSortAndWrite12BitNumbers("..//..//samples//test3.bin", "..//..//samples//test3.out");
    TestReadSortAndWrite12BitNumbers("..//..//samples//test4.bin", "..//..//samples//test4.out");
    TestReadSortAndWrite12BitNumbers("..//..//samples//test5.bin", "..//..//samples//test5.out");
    TestReadSortAndWrite12BitNumbers("..//..//samples//test6.bin", "..//..//samples//test6.out");
    TestReadSortAndWrite12BitNumbers("..//..//samples//test7.bin", "..//..//samples//test7.out");
    getchar();

}
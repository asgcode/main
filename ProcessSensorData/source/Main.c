#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"
#include "FileIO.h"
#include "Knobs.h"
#include "ProcessSensor.h"


INT TestReadSortAndWrite12BitNumbers(const char* pInputFileName, const char* pOutputFileName)
{
    FILE* hInputFile;
    FILE* hOutputFile;
    UINT  totalSizeInBytes, allocCBufSizeInBytes;
    UINT  totalNumEntries;
    CircularBuffer cReadBuf;
    CircularBuffer cProcessedBuf;
    BOOL status = TRUE;
    const UINT maxCBufSizeInBytes = (MaxNumOfBufEntries * NumBitsPerEntries)/(NumBitsPerByte);

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
        totalSizeInBytes = GetFileSizeInBytes(hInputFile);
        memset(&cReadBuf, 0x0, sizeof(CircularBuffer));
        memset(&cProcessedBuf, 0x0, sizeof(CircularBuffer));

        totalNumEntries    = (totalSizeInBytes * NumBitsPerByte)/(NumBitsPerEntries);
        // Maximum we should allocate 12 DWORDs for each read and data processing circular buffer
        allocCBufSizeInBytes = (totalSizeInBytes > maxCBufSizeInBytes) ? maxCBufSizeInBytes : totalSizeInBytes;

        cReadBuf.numEntries = (totalNumEntries > MaxNumOfBufEntries) ? MaxNumOfBufEntries : totalNumEntries;

        cReadBuf.pData = (UCHAR*) malloc(allocCBufSizeInBytes * sizeof(UCHAR));
        if (cReadBuf.pData == NULL)
        {
            printf("ERROR: Out of memory");
            status = FALSE;
        }
        cProcessedBuf.pData = (UCHAR*) malloc(allocCBufSizeInBytes * sizeof(UCHAR));
        if (cProcessedBuf.pData == NULL)
        {
            printf("ERROR: Out of memory");
            status = FALSE;
        }
    }

    if (status == TRUE)
    {
        UINT readSize, lastReadEntries;

        while (totalSizeInBytes != 0)
        {
            readSize = (totalSizeInBytes > allocCBufSizeInBytes) ? allocCBufSizeInBytes : totalSizeInBytes;
            ReadInput(hInputFile, cReadBuf.pData, readSize);

            ProcessSensorData(&cReadBuf, &cProcessedBuf);
            totalSizeInBytes -= readSize;
        }

        WriteOutput(hOutputFile, TRUE, &cProcessedBuf);

        lastReadEntries = (readSize * NumBitsPerByte)/(NumBitsPerEntries);

        // Move tail to just after the last read entries if
        if (totalNumEntries > MaxNumOfBufEntries)
        {
            // Set 
            cReadBuf.tail = lastReadEntries % MaxNumOfBufEntries;
        }

        WriteOutput(hOutputFile, FALSE, &cReadBuf);
    }

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
    getchar();

}
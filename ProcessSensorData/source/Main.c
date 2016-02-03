#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"
#include "FileIO.h"
#include "Knobs.h"
#include "ProcessSensor.h"


int main()
{
    FILE* hInputFile;
    FILE* hOutputFile;
    UINT  sizeInBytes, maxCBufSizeInBytes, processCBufSizeInBytes;
    UINT  totalNumEntries;
    CircularBuffer cReadBuf;
    CircularBuffer cProcessedBuf;
    BOOL status = TRUE;

    hInputFile = OpenFile("C:\\code\\asgcode\\main\\ProcessSensorData\\samples\\test1.bin", "rb");
    if (hInputFile == NULL)
    {
        printf("ERROR: Failed to open input file %s", __LINE__);
        status = FALSE;
    }

    hOutputFile = OpenFile("C:\\code\\asgcode\\main\\ProcessSensorData\\samples\\test1.out", "wb");
    if (hOutputFile == NULL)
    {
        printf("ERROR: Failed to open output file %s", __LINE__);
        status = FALSE;
    }

    if (status == TRUE)
    {
        sizeInBytes = GetFileSizeInBytes(hInputFile);
        memset(&cReadBuf, 0x0, sizeof(CircularBuffer));
        memset(&cProcessedBuf, 0x0, sizeof(CircularBuffer));

        maxCBufSizeInBytes = (MaxNumOfBufEntries * NumBitsPerEntries)/(NumBitsPerByte);
        totalNumEntries    = (sizeInBytes * NumBitsPerByte)/(NumBitsPerEntries);

        processCBufSizeInBytes = (sizeInBytes > maxCBufSizeInBytes) ? maxCBufSizeInBytes : sizeInBytes;

        cReadBuf.pData = (UCHAR*) malloc(sizeInBytes * sizeof(UCHAR));
        if (cReadBuf.pData == NULL)
        {
            printf("ERROR: Out of memory");
            status = FALSE;
        }
        cProcessedBuf.pData = (UCHAR*) malloc(sizeInBytes * sizeof(UCHAR));
        if (cProcessedBuf.pData == NULL)
        {
            printf("ERROR: Out of memory");
            status = FALSE;
        }
    }

    if (status == TRUE)
    {
        UINT readSize, readEntries;

        while (sizeInBytes != 0)
        {
            readSize = (sizeInBytes > processCBufSizeInBytes) ? processCBufSizeInBytes : sizeInBytes;
            ReadInput(hInputFile, cReadBuf.pData, readSize);

            ProcessSensorData(cReadBuf.pData, readSize, &cProcessedBuf);
            sizeInBytes -= readSize;
        }

        readEntries = (readSize * NumBitsPerByte)/(NumBitsPerEntries);

        cReadBuf.numEntries = (totalNumEntries > MaxNumOfBufEntries) ? MaxNumOfBufEntries : readEntries;
        if (totalNumEntries > MaxNumOfBufEntries)
        {
            cReadBuf.tail = readEntries % MaxNumOfBufEntries;
        }

        WriteOutput(hOutputFile, TRUE, &cProcessedBuf);
        WriteOutput(hOutputFile, FALSE, &cReadBuf);
    }

    CloseFile(hOutputFile);
    CloseFile(hInputFile);
    free(cReadBuf.pData);
    // free(cReadBuf.pData);

}
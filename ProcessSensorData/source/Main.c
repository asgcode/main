#include <stdio.h>
#include "Utils.h"
#include "FileIO.h"
#include "Knobs.h"
#include "ProcessSensor.h"


int main()
{
    FILE* hInputFile;
    UINT  sizeInBytes, maxCBufSizeInBytes, processCBufSizeInBytes;
    UINT  totalNumEntries;
    CircularBuffer cReadBuf;
    CircularBuffer cProcessedBuf;
    BOOL status = TRUE;

    hInputFile = OpenFile("test1.bin", "rb");
    if (hInputFile == NULL)
    {
        printf("ERROR: Failed to open input file");
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

        cReadBuf.pData = (UCHAR*) malloc(sizeInBytes, sizeof(UCHAR));
        if (cReadBuf.pData)
        {
            printf("ERROR: Out of memory");
            status = FALSE;
        }
    }

    if (status == TRUE)
    {
        while (sizeInBytes != 0)
        {

        }
    }

    CloseFile(hInputFile);
    free(cReadBuf.pData);

}
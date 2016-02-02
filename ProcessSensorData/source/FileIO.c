#include <stdio.h>
#include "BitUtils.h"
#include "Utils.h"

FILE* OpenFile(const char* pFileName, const char* mode)
{
    return fopen(pFileName, mode);;
}

void CloseFile(FILE* hFile)
{
  fclose(hFile);
}

UINT GetFileSizeInBytes(FILE* hFile)
{
    fseek(hInFile, 0, SEEK_END);
    fileSizeInBytes = ftell(hInFile);
    rewind(hInFile);
}

UINT ReadInput(FILE*  hFile,
               UCHAR* pData,
               UINT   readSizeInBytes)
{
    return fread(pData, sizeof(UCHAR), readSizeInBytes, hFile);
}

UINT WriteOutput(FILE*           hFile,   ///TODO parameter comments
                 BOOL            sorted,
                 CircularBuffer* pCBuf)
{
    FILE* hInFile    = NULL;
    UINT  currentPos;
    UINT  numOfEntriesToWrite;

    if (sorted)
    {
        fputs("--Sorted Max 32 Values--\r\n", hInFile);
    }
    else
    {
        fputs("--Last 32 Values--\r\n", hInFile);
    }

    currentPos          = pCBuf->tail;
    numOfEntriesToWrite = pCBuf->numEntries;

    while (numOfEntriesToWrite > 0)
    {
        fprintf(hInFile, "%d \r\n", Get12BitEntry(pCBuf->pData, currentPos));
        currentPos = ((currentPos + 1) % pCBuf->numEntries;
        numOfEntriesToWrite--;
    }
}

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

UINT GetFileEntries(FILE* hFile)
{
    UINT fileSizeInBytes;

    fseek(hFile, 0, SEEK_END);
    fileSizeInBytes = ftell(hFile);
    rewind(hFile);

    return ConvertBytesToEntries(fileSizeInBytes);
}

UINT ReadInput(FILE*  hFile,
               CircularBuffer* pCBuf)
{

    return fread(pCBuf->pData, sizeof(UCHAR), ConvertEntriesToByte(pCBuf->numEntries), hFile);
}

VOID WriteOutput(FILE*           hFile,   ///TODO parameter comments
                 BOOL            isSorted,
                 CircularBuffer* pCBuf)
{
    UINT  currentPos;
    UINT  numOfEntriesToWrite;

    if (isSorted)
    {
        fputs("--Sorted Max 32 Values--\r\n", hFile);
    }
    else
    {
        fputs("--Last 32 Values--\r\n", hFile);
    }

    currentPos          = pCBuf->tail;
    numOfEntriesToWrite = pCBuf->numEntries;

    while (numOfEntriesToWrite > 0)
    {
        fprintf(hFile, "%d \r\n", Get12BitEntry(pCBuf->pData, currentPos));
        currentPos = (currentPos + 1) % pCBuf->numEntries;
        numOfEntriesToWrite--;
    }
}

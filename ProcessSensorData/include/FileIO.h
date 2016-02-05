#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include "Utils.h"

FILE* OpenFile(const char* pFileName, const char* mode);
void  CloseFile(FILE* hFile);
UINT  GetFileEntries(FILE* hFile);
VOID  WriteOutput(FILE* hFile, BOOL isSorted, CircularBuffer* pCBuf);
UINT  ReadInput(FILE* hFile, CircularBuffer* pCBuf);

#endif
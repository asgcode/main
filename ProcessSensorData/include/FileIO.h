#ifndef FILEIO_H
#define FILEIO_H

#include "Utils.h"

FILE* OpenFile(const char* pFileName, const char* mode);
void  CloseFile(FILE* hFile);
UINT  GetFileSizeInBytes(FILE* hFile);
UINT  WriteOutput(const char* pWriteFileName, BOOL reopen, CircularBuffer* pCBuf);
UINT  ReadInput(const char* pReadFileName, CircularBuffer* pCBuf);

#endif
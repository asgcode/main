#ifndef FILEIO_H
#define FILEIO_H

#include "Utils.h"

UINT WriteOutput(const char* pWriteFileName, BOOL reopen, CircularBuffer* pCBuf);
UINT ReadInput(const char* pReadFileName, CircularBuffer* pCBuf);

#endif
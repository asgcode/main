/*******************************************************************************************************************************
 *  @file   FileIO.h
 *  @brief  Provides functions to perform file I\O to\from circular buffers.
 *  @author Amit Bansal
 *
 *
 *******************************************************************************************************************************/
#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include "CommonUtils.h"

// Just wrapper APIs to keep fileIO code at one place
FILE* OpenFile(const char* pFileName, const char* mode);
void  CloseFile(FILE* hFile);

// Returns number of 32 bit unsigned entries in a given file handle obtained
// from a previous file open
UINT  GetFileEntries(FILE* hFile);

// Write provided string to a given file handle
VOID  WriteStringToOutput(FILE* hFile, const char* pString);

// Write given circular buffer containing 12 bit entries to the given file handle
VOID  WriteCBufToOutput(FILE* hFile, CircularBuffer* pCBuf);

// Read specified number of 12 bit entries in a given circular buffer, client responsible for allocating
// circular buffer data pointer
UINT  ReadInput(FILE* hFile, UINT numEnries, CircularBuffer* pCBuf);

#endif
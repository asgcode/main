#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include "Utils.h"

USHORT Get12BitEntry(UCHAR* pByte, UINT position);
VOID Set12BitEntry(UCHAR* pByte, UINT position, USHORT value);
UINT ConvertBytesToEntries(UINT numBytes);
UINT ConvertEntriesToByte(UINT numEntries);

#endif
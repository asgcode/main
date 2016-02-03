#ifndef UTILS_H
#define UTILS_H

typedef unsigned char UCHAR;
typedef unsigned int  UINT;
typedef short SHORT;
typedef int INT;

typedef enum
{
    FALSE = 0,
    TRUE  = 1
}BOOL;

typedef struct _CircularBuffer
{
    UCHAR* pData;
    UINT   head;
    UINT   tail;
    UINT   numEntries;
    UINT   entrySizeinBits;
}CircularBuffer;

#endif

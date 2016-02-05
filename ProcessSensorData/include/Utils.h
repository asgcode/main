#ifndef UTILS_H
#define UTILS_H

typedef unsigned char UCHAR;
typedef unsigned int  UINT;
typedef short SHORT;
typedef unsigned short USHORT;
typedef int INT;
typedef void VOID; 

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
}CircularBuffer;

#endif

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

/**
 * @brief  Number of entries which needs to be stored in circular buffer
 *
 */
enum
{
    MaxNumOfBufEntries = 32
};

/**
 * @brief Number of bits per entry
 *
 */
enum
{
    NumBitsPerEntries = 12
};

/**
 * @brief Number of bits per Byte
 *
 */
enum
{
    NumBitsPerByte = 8
};
/**
 * @brief Standard assumption on number of bits per byte
 *
 */
enum
{
    NumBitsInByte = 8
};

typedef struct _CircularBuffer
{
    UCHAR* pData;
    UINT   head;
    UINT   tail;
    UINT   numEntries;
}CircularBuffer;

// Retrievs a 12 bit entry at a given location from a given packed byte array.
// Client responsible for providing valid data pointer and ensuring data is in correct layout i.e.
// it has back to back 12 bit entries without any intermediate gaps.
USHORT Get12BitEntry(UCHAR* pByte, UINT position);

// Sets a given 12 bit entry value to the given position of the byte array. Client responsible
// for ensuring validity of data input
VOID Set12BitEntry(UCHAR* pByte, UINT position, USHORT value);

// Convert given number of bytes to 12 bits entries assuming packed data layout
UINT ConvertBytesToEntries(UINT numBytes);

// Convert given number of 12 bit entries to number of bytes required for storing those 12 bit entries
UINT ConvertEntriesToByte(UINT numEntries);

#endif

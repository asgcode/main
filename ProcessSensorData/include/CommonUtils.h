/*******************************************************************************************************************************
 *  @file   CommonUtils.h
 *  @brief  Provides some generic types as well as declares some routines to perform bit manipulation on circular buffer
 *          containing 12 bit entries
 *  @author Amit Bansal
 *
 *
 *******************************************************************************************************************************/
#ifndef UTILS_H
#define UTILS_H

// Some tydefs just to make life easy or to increase portability whenever it will be required
typedef unsigned char UCHAR;
typedef unsigned int  UINT;
typedef short SHORT;
typedef unsigned short USHORT;
typedef int INT;
typedef void VOID;

// Convert given number of bytes to 12 bits entries assuming packed data layout i.e. back to back 12 bit numbers
#define CONVERT_BYTES_TO_ENTRIES(X) ((X * NumBitsPerByte)/(NumBitsPerEntries))

// Convert given number of 12 bit entries to number of bytes required for storing those 12 bit entries
#define CONVERT_ENTRIES_TO_BYTE(X) (((X & 1) == 0) ? \
        ((X * NumBitsPerEntries)/(NumBitsPerByte)) : (((X * NumBitsPerEntries)/(NumBitsPerByte)) + 1))

// Our own BOOL as ANSI C doesn't provide one
typedef enum
{
    FALSE = 0,
    TRUE  = 1
}BOOL;

/**
 * @brief Maximum number of entries which needs to be stored in circular buffer
 *
 */
enum
{
    MaxNumOfBufEntries = 32
};

/**
 * @brief Number of bits per entry of circular buffer
 *
 */
enum
{
    NumBitsPerEntries = 12
};

/**
 * @brief Number of bits per byte
 *
 */
enum
{
    NumBitsPerByte = 8
};

/**
 * @brief Number of bits per nibble
 *
 */
enum
{
    NumBitsPerNibble = 4
};


typedef struct _CircularBuffer
{
    UCHAR* pData;       ///< Pointer to circular buffer content
    UINT   head;        ///< Right most entry in the circular buffer
    UINT   tail;        ///< Left most entry in the circular buffer
    UINT   numEntries;  ///< Number of current entries in the circular buffer
}CircularBuffer;

// Retrievs a 12 bit entry at a given location from a given packed byte array.
// Client responsible for providing valid data pointer and ensuring data is in correct layout i.e.
// it has back to back 12 bit entries without any intermediate gaps.
USHORT Get12BitEntry(UCHAR* pByte, UINT position);

// Sets a given 12 bit entry value to the given position of the byte array. Client responsible
// for ensuring validity of data input
VOID Set12BitEntry(UCHAR* pByte, UINT position, USHORT value);

#endif

/*******************************************************************************************************************************
 *  @file   CommonUtils.c
 *  @brief  Defines bit manipulation utility routines to retrieve and set 12 bit unsigned numbers from or to a byte array.
 *  @author Amit Bansal
 *
 *
 *******************************************************************************************************************************/
#include "CommonUtils.h"

/*******************************************************************************************************************************
*   Get12BitEntry
*
*   @brief
*       Retrieves 12 Bit number from the given position of the byte array
*
*   @return
*       12 bit enry value
*
*******************************************************************************************************************************/
USHORT Get12BitEntry(
                     UCHAR* pByte,             ///< pointer to the 12 bit entries array
                     UINT   position)          ///< location from which 12 bit number needs to be retrieved
{
    UINT index = ((position) * 12)/8;

    // In a packed 12 bit array, even position are at first 12 bit of 3 bytes
    // and odd position are at second 12 bit of same 3 byte and this pattern
    // keeps repeating itself after every 3 byte.
    if ((position & 1) == 0)
    {
        // To form 12 bit entry on even position take everything from first byte
        // and only the left nibble of the second byte.
        return ((pByte[index] << 4) |
                ((pByte[index + 1] & (0xF0)) >> 4));
    }
    else
    {
        // To form 12 bit entries on odd position, take only right nibble from first
        // byte and complete byte from the next byte
        return (((pByte[index] & 0xF) << 8) | pByte[index + 1]);
    }
}

/*******************************************************************************************************************************
*   Set12BitEntry
*
*   @brief
*      Writes the least most 12 bit of the given number at the given position in the given byte array
*
*   @return
*       12 bit enry value
*
*******************************************************************************************************************************/
VOID Set12BitEntry(
                   UCHAR* pByte,        ///< pointer to array of 12 bit unsigned entries
                   UINT   position,     ///< position at which given number needs to be written
                   USHORT value)        ///< value which needs to be written
{
    UINT index = ((position) * 12)/8;

    if ((position & 1) == 0)
    {
        pByte[index] = (value & 0xFF0) >> 4;
        pByte[index + 1] = (pByte[index + 1] & 0x0F) | ((value & 0xF) << 4);
        
    }
    else
    {
        pByte[index] = (pByte[index] & 0xF0) | ((value & 0xF00) >> 8);
        pByte[index + 1] = value & 0xFF;
    }

}

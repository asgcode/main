#include "BitUtils.h"


USHORT Get12BitEntry(UCHAR* pByte, UINT position)
{
    int index = ((position) * 12)/8;
    if (position % 2 == 0)
    {
        // Odd numbers are at first position of byte
        return ((pByte[index] << 4) | ((pByte[index + 1] & (0xF << 4)) >> 4));
    }
    else
    {
        //Even numbers are at second half of byte
        return (((pByte[index] & 0xF) << 8) | pByte[index + 1]);
    }
}
VOID Set12BitEntry(UCHAR* pByte, UINT position, USHORT value)
{
    int index = ((position) * 12)/8;
    if (position % 2 == 0)
    {
        // Odd numbers's right most nibble would go in
        // consequtive byte's left nibble.
        pByte[index] = (value & 0xFF0) >> 4;
        pByte[index + 1] = (pByte[index + 1] & 0x0F) | ((value & 0xF) << 4);
        
    }
    else
    {
        // Even numbers's left most nibble is at second nibble of first byte
        // and remaining byte at complete next index
        pByte[index] = (pByte[index] & 0xF0) | ((value & 0xF00) >> 8);
        pByte[index + 1] = value & 0xFF;
    }

}
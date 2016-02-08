/*******************************************************************************************************************************
 *  @file   FileIO.c
 *  @brief  Defines routines to perform file input and output to and from a circular buffer.
 *  @author Amit Bansal
 *
 *
 *******************************************************************************************************************************/

#include <stdio.h>
#include "CommonUtils.h"

/*******************************************************************************************************************************
*   OpenFile
*
*   @brief
*       Wrapper of file open API
*
*   @return
*       File Handle if successful otherwise NULL
*
*******************************************************************************************************************************/
FILE* OpenFile(const char* pFileName,      ///< [in] File name to be opened
               const char* mode)           ///< [in] read or write mode
{
    return fopen(pFileName, mode);
}

/*******************************************************************************************************************************
*   CloseFile
*
*   @brief
*       Wrapper of file close API
*
*   @return
*       N/A
*
*******************************************************************************************************************************/
VOID CloseFile(
               FILE* hFile)      ///< [in] File handle to be closed
{
  fclose(hFile);
}

/*******************************************************************************************************************************
*   GetFileEntries
*
*   @brief
*       Use file IO APIs to query file size and return value in terms of total number of 12 bit entries
*
*   @return
*       12 bit entries
*
*******************************************************************************************************************************/
UINT GetFileEntries(
                    FILE* hFile) ///< [in] File handle in which number of entries needs to be looked up
{
    UINT fileSizeInBytes;

    fseek(hFile, 0, SEEK_END);
    fileSizeInBytes = ftell(hFile);
    rewind(hFile);

    return ConvertBytesToEntries(fileSizeInBytes);
}

/*******************************************************************************************************************************
*   ReadInput
*
*   @brief
*       Read specified number of 12 bit entries in a given circular buffer using FILE IO APIs
*
*   @return
*       -1 upon failure otherwise 0 upon success
*
*******************************************************************************************************************************/
INT ReadInput(FILE*           hFile,               //< [in]  file handle of read file
              UINT            numEnriesToBeRead,   //< [in]  number of 12 bit entries require to be read
              CircularBuffer* pOutCBuf)            //< [out] Output circular buffer in which data was read
{
    UINT readBytes, readEntries;
    BOOL isCBufEmpty       = FALSE;

    if ((pOutCBuf->tail == 0) && (pOutCBuf->head == pOutCBuf->tail))
    {
        isCBufEmpty = TRUE;
    }

    readBytes   = fread(pOutCBuf->pData, sizeof(UCHAR), ConvertEntriesToByte(numEnriesToBeRead), hFile);
    readEntries = ConvertBytesToEntries(readBytes);

    if (readEntries != numEnriesToBeRead)
    {
        printf("ERROR: Couldn't read expected number of entries");
        return -1;
    }

    // If CBuf was already full then tail should point to oldest entry
    if (isCBufEmpty == FALSE)
    {
        pOutCBuf->tail = (readEntries % MaxNumOfBufEntries);
    }

    // head should always point to right most valid entry in CBuf
    pOutCBuf->head       = ((readEntries - 1) % MaxNumOfBufEntries);
    pOutCBuf->numEntries = numEnriesToBeRead;

    return 0;
}

/*******************************************************************************************************************************
*   ReadInput
*
*   @brief
*       Read specified number of 12 bit entries in a given circular buffer using FILE IO APIs
*
*   @return
*       N/A
*
*******************************************************************************************************************************/

VOID WriteStringToOutput(FILE*           hFile,   ///[in] handle of file in which string needs to be written
                         const char*     pString) ///[in] input string pointer which needs to be written
{
    if (pString != NULL)
    {
        fputs(pString, hFile);
    }
}

/*******************************************************************************************************************************
*   WriteCBufToOutput
*
*   @brief
*       Write 12 bit enries from given circular buffer to a file using FILE IO APIs
*
*   @return
*       N/A
*
*******************************************************************************************************************************/
VOID WriteCBufToOutput(FILE*           hFile,   ///[in] handle of file in which string needs to be written
                       CircularBuffer* pCBuf)   ///[in] pointer to circular buffer from which data needs to be written
{
    UINT  currentPos;
    UINT  numOfEntriesToWrite;

    currentPos          = pCBuf->tail;
    numOfEntriesToWrite = pCBuf->numEntries;

    while (numOfEntriesToWrite > 0)
    {
        fprintf(hFile, "%d \r\n", Get12BitEntry(pCBuf->pData, currentPos));
        currentPos = (currentPos + 1) % pCBuf->numEntries;
        numOfEntriesToWrite--;
    }
}

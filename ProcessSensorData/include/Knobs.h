/** @file Knobs.h
 *  @brief Configurable settings for processing sensor data
 *
 *  This file contains enums, some of which can be tuned
 *  as per the data processing requirements
 *
 */

#ifndef KNOBS_H
#define KNOBS_H

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
}

/**
 * @brief Number of bits per Byte
 *
 */
enum
{
    NumBitsPerByte = 8
}
/**
 * @brief Standard assumption on number of bits per byte
 *
 */
enum
{
    NumBitsInByte = 8
}

#endif
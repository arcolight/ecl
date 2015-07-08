/**
 * @file
 *
 * @brief Sized data struct.
 * @details Small struct to pass data with data size as one object.
 *
 * @ingroup ecl
 */
#ifndef ECL_SIZED_DATA_H
#define ECL_SIZED_DATA_H

#include <stdint.h>
#include <stdio.h>

/**
 * @brief sized_data struct
 *
 */
struct sized_data
{
    /**
     * @brief constant pointer to data
     */
    uint8_t* const ptr;
    /**
     * @brief size of data
     */
    size_t         size;
};

/**
 * @brief const version of @ref sized_data struct
 *
 */
struct const_sized_data
{
    /**
     * @brief constant pointer to constant data
     */
    const uint8_t* const ptr;
    /**
     * constant data size
     */
    const size_t         size;
};

#endif // ECL_SIZED_DATA_H

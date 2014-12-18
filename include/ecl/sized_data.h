#ifndef ECL_SIZED_DATA_H
#define ECL_SIZED_DATA_H

#include <stdint.h>
#include <stdio.h>

struct sized_data
{
    uint8_t* const ptr;
    size_t         size;
};

struct const_sized_data
{
    const uint8_t* const ptr;
    const size_t         size;
};

#endif // ECL_SIZED_DATA_H

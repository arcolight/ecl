#ifndef ECL_DATA_H
#define ECL_DATA_H

#include "stdint.h"
#include "stdio.h"

typedef struct data {
    uint8_t* const ptr;
    size_t         size;
} data_t;

#endif // ECL_DATA_H

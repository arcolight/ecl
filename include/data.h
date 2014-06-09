#ifndef DATA_H
#define DATA_H

#include "stdint.h"
#include "stdio.h"

typedef struct data {
    uint8_t* const ptr;
    size_t         size;
} data_t;

#endif // DATA_H

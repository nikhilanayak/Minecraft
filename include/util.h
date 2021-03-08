#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>


#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256

#define CUBE_SIZE 0.5f

typedef struct{
    uint64_t x;
    uint64_t y;
    uint64_t z;
} point;

#endif
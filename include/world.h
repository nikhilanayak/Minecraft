#ifndef WORLD_H
#define WORLD_H

#include "../include/chunk.h"

typedef struct{
    chunk** chunks;
} world;

void add_chunk_by_chunkpos(int64_t x,  int64_t y);
void add_chunk_by_worldpos(int64_t x,  int64_t y);

void world_set_block(world* c, int64_t x, int64_t y, int64_t z, uint8_t state);
u world_get_block(chunk* c, uint16_t x, uint16_t y, uint16_t z);

#endif
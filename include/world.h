#ifndef WORLD_H
#define WORLD_H

#include "../include/chunk.h"
#include "../include/util.h"
#include "../include/datastructures/hashmap.h"

typedef struct{
    struct hashmap* data;

} world;


void init_world(world* w);
void deinit_world(world* w);

void add_chunk_from_worldpos(world* w, int64_t x, int64_t z);
void add_chunk_from_chunkpos(world* w, int64_t x,  int64_t z);


chunk* world_get_chunk(world* w, int64_t x, int64_t z);
void world_set_block(world* w, i64_coord *pos, blockstate state);
blockstate world_get_block(world* w, uint16_t x, uint16_t y, uint16_t z);

void render_chunk_at_world_pos(world* w, i64_coord* pos, mat4 model, GLuint modelLoc);
void render_chunk_at_chunk_pos(world *w, int64_t x, int64_t z, mat4 model, GLuint modelLoc);
i64_coord world_pos_to_chunk_pos(const i64_coord* c);

void trend(world* w, mat4 model, GLuint modelLoc);

chunk test[20][20];


#endif
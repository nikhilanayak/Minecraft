#include "../include/world.h"



int chunk_compare(const void *a_, const void *b_, const void *udata) {
	const chunk *a = a_;
	const chunk *b = b_;

	return (a->pos[0] == b->pos[0] && a->pos[1] == b->pos[1]) ? 0 : -1;
}

uint64_t chunk_hash(const void *item, uint64_t seed0, uint64_t seed1) {
	const chunk *c = item;
	return hashmap_murmur(c->pos, sizeof(int64_t) * 2, seed0, seed1);
}

bool chunk_iter(const void* item, void* udata){
    chunk* c = item;
    free(c);
    return true;
}

void init_world(world *w) {
	w->data = hashmap_new(sizeof(chunk), 0, 0, 0, chunk_hash, chunk_compare, NULL);
}
void deinit_world(world *w) {
    //hashmap_scan(w->data, chunk_iter, NULL);
	hashmap_free(w->data);
}

void add_chunk_from_worldpos(world *w, int64_t x, int64_t z) {
	i64_coord chunk_pos = world_pos_to_chunk_pos(&(const i64_coord){.x = x, .y = 0, .z = z});
	add_chunk_from_chunkpos(w, chunk_pos.x, chunk_pos.z);
}
void add_chunk_from_chunkpos(world *w, int64_t x, int64_t z) {
	//chunk *c = malloc(sizeof(chunk));
    chunk c;
	init_chunk(&c, x, z);

#define off 1
	for (int _x = 0; _x < CHUNK_SIZE; _x += off) {
		for (int _y = 0; _y < CHUNK_HEIGHT/2; _y += off) {
			for (int z = 0; z < CHUNK_SIZE; z += off) {
				c.data[_x][_y][z] = 1;
			}
		}
	}
    c.data[0][0][0] = 1;

	build_mesh(&c);

	hashmap_set(w->data, &c);
}

void world_set_block(world *w, i64_coord *pos, blockstate state) {
	i64_coord c_pos = world_pos_to_chunk_pos(&(const i64_coord){pos->x, pos->y, pos->z});
	chunk *c = world_get_chunk(w, c_pos.x, c_pos.z);

    if(c->data[mod(pos->x, CHUNK_SIZE)][pos->y][mod(pos->z, CHUNK_SIZE)] == state) return;

    c->data[mod(pos->x, CHUNK_SIZE)][pos->y][mod(pos->z, CHUNK_SIZE)] = state;

	build_mesh(c);
}
blockstate world_get_block(world *w, uint16_t x, uint16_t y, uint16_t z) {
	i64_coord c_pos = world_pos_to_chunk_pos(&(const i64_coord){x, y, z});
	chunk *c = world_get_chunk(w, c_pos.x, c_pos.z);
	return c->data[x][y][z];
}

void render_chunk_at_chunk_pos(world *w, int64_t x, int64_t z, mat4 model, GLuint modelLoc) {
	chunk *c = world_get_chunk(w, x, z);
	if (c == NULL) {
		add_chunk_from_chunkpos(w, x, z);
	}

	c = hashmap_get(w->data, &(chunk){.pos = {x, z}});

	render_chunk(c, model, modelLoc);
}

void render_chunk_at_world_pos(world *w, i64_coord *pos, mat4 model, GLuint modelLoc) {

	i64_coord c_pos = world_pos_to_chunk_pos(pos);

	render_chunk_at_chunk_pos(w, c_pos.x, c_pos.z, model, modelLoc);
}

i64_coord world_pos_to_chunk_pos(const i64_coord *c) {
	return (i64_coord){
		.x = (c->x >= 0) ? c->x / CHUNK_SIZE : (c->x + 1) / CHUNK_SIZE - 1,
		.y = c->y,
		.z = (c->z >= 0) ? c->z / CHUNK_SIZE : (c->z + 1) / CHUNK_SIZE - 1};
}

chunk *world_get_chunk(world *w, int64_t x, int64_t z) {
	chunk *c = hashmap_get(w->data, &(chunk){.pos = {x, z}});
	if (c == NULL) {
		add_chunk_from_chunkpos(w, x, z);
		c = hashmap_get(w->data, &(chunk){.pos = {x, z}});
	}
	return c;
}

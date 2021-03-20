#include <stdio.h>
#include "../include/datastructures/hashmap.h"
#include "../include/util.h"

typedef struct{
    i64_coord pos;
    int val;
} chunk_map;

int chunk_cmp(const void* _a, const void* _b, const void* udata){
    chunk_map* a = (chunk_map*)_a;
    chunk_map* b = (chunk_map*)_b;

    if(a->pos.x == b->pos.x && a->pos.y == b->pos.y && a->pos.z == b->pos.z){
        return 0;
    }
    return -1;
}

uint64_t chunk_hash(const void* item, uint64_t seed0, uint64_t seed1){
    const chunk_map* m = (const chunk_map)item;
    
    return hashmap_sip(&(m->pos), sizeof(m->pos), seed0, seed1);
}


void test_map(){
    struct hashmap* map = hashmap_new(sizeof(chunk_map), 0, 0, 0, chunk_hash, chunk_cmp, NULL);
    hashmap_set(map, &(chunk_map){.pos = {.x = 0, .y = 0, .z = 0}, .val = 10});
    hashmap_set(map, &(chunk_map){.pos = {.x = 0, .y = 1, .z = 0}, .val = 11});

    chunk_map* got = hashmap_get(map, &(chunk_map){.pos = {.x = 0, .y = 0, .z = 0}});

    printf("-> %d\n", got->val);
}
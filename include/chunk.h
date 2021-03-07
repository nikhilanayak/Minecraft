#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>
#include <string.h>
#include <time.h>

#include <glad/glad.h>

#include "../include/cglm/cglm.h"
#include "../include/cglm/mat4.h"
#include "../include/cglm/vec3.h"

#include "../include/shader.h"
#include "../include/gl_vert.h"
#include "../include/vec.h"
#include "../include/util.h"

//#define CHUNK_SIZE 16
//#define CHUNK_HEIGHT 32

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256


#define X_POS 0
#define X_MIN 1
#define Y_POS 2
#define Y_MIN 3
#define Z_POS 4
#define Z_MIN 5




typedef struct{
    point pos;
    uint8_t type;
} mesh;

typedef struct{
    int8_t data[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    unsigned int VAO;
    unsigned int VBO;
    unsigned int VBO_len;
} chunk;

void build_mesh(chunk *c);
void chunk_init(chunk* c);
void render_chunk(chunk* c, vert* verts, unsigned int program, mat4 model, unsigned int modelLoc);

#endif
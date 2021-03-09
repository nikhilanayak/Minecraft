#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>
#include <string.h>
#include <time.h>

#include <glad/glad.h>

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/mat3.h>

#include "../include/shader.h"
#include "../include/gl_vert.h"
#include "../include/vec.h"
#include "../include/util.h"



#define X_POS 0
#define X_MIN 1
#define Y_POS 2
#define Y_MIN 3
#define Z_POS 4
#define Z_MIN 5

typedef struct{
    int8_t data[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    unsigned int VAO;
    unsigned int VBO;
    unsigned int VBO_len;

} chunk;

void build_mesh(chunk *c);
void chunk_init(chunk* c);
void render_chunk(chunk *c, mat4 model, unsigned int modelLoc, GLuint tex1, GLuint tex2, vert* vert);
bool fsave_chunk(chunk* c, char* file_name);
bool fload_chunk(chunk* c, char* file_name);

#endif
#include "../include/chunk.h"
#include <stdio.h>

#define ping fprintf(stderr, "ping from %d\n", __LINE__);

void chunk_init(chunk *c) {
    memset(c->data, 0, sizeof(uint8_t) * CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE);
}


#define addrow(vec, x, y, z, u, v) \
    vec_push(&vec, x);\
    vec_push(&vec, y);\
    vec_push(&vec, z);\
    vec_push(&vec, u);\
    vec_push(&vec, z);\


void build_mesh(chunk *c) {
    fprintf(stderr, "meshing: ");

    vec_t(float) verts;
    vec_init(&verts);

    unsigned int num_verts = 0;

    float CUBE_SIZE_X;
    float CUBE_SIZE_Y;
    float CUBE_SIZE_Z;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (c->data[x][y][z] == 1) {
                    CUBE_SIZE_X = CUBE_SIZE + x;
                    CUBE_SIZE_Y = CUBE_SIZE + y;
                    CUBE_SIZE_Z = CUBE_SIZE + z;

                    if (x - 1 < 0 || c->data[x - 1][y][z] == 0) {
                        addrow(verts, -CUBE_SIZE + x, CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 0.0f);
                        addrow(verts, -CUBE_SIZE + x, CUBE_SIZE + y, -CUBE_SIZE + z, 1.0f, 1.0f);
                        addrow(verts, -CUBE_SIZE + x, -CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 1.0f);
                        addrow(verts, -CUBE_SIZE + x, -CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 1.0f);
                        addrow(verts, -CUBE_SIZE + x, -CUBE_SIZE + y, CUBE_SIZE + z, 0.0f, 0.0f);
                        addrow(verts, -CUBE_SIZE + x, CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 0.0f);
                        num_verts += 6;
                    }

                    if (x + 1 >= CHUNK_SIZE || c->data[x + 1][y][z] == 0) {
                        addrow(verts, CUBE_SIZE + x, CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 0.0f);
                        addrow(verts, CUBE_SIZE + x, CUBE_SIZE + y, -CUBE_SIZE + z, 1.0f, 1.0f);
                        addrow(verts, CUBE_SIZE + x, -CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 1.0f);
                        addrow(verts, CUBE_SIZE + x, -CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 1.0f);
                        addrow(verts, CUBE_SIZE + x, -CUBE_SIZE + y, CUBE_SIZE + z, 0.0f, 0.0f);
                        addrow(verts, CUBE_SIZE + x, CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 0.0f);
                        num_verts += 6;
                    }

                    if (y - 1 < 0 || c->data[x][y - 1][z] == 0) {
                        addrow(verts, -CUBE_SIZE + x, -CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 1.0f);
                        addrow(verts, CUBE_SIZE + x, -CUBE_SIZE + y, -CUBE_SIZE + z, 1.0f, 1.0f);
                        addrow(verts, CUBE_SIZE + x, -CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 0.0f);
                        addrow(verts, CUBE_SIZE + x, -CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 0.0f);
                        addrow(verts, -CUBE_SIZE + x, -CUBE_SIZE + y, CUBE_SIZE + z, 0.0f, 0.0f);
                        addrow(verts, -CUBE_SIZE + x, -CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 1.0f);
                        num_verts += 6;
                    }

                    if (y + 1 >= CHUNK_HEIGHT || c->data[x][y + 1][z] == 0) {
                        addrow(verts, -CUBE_SIZE + x, CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 1.0f);
                        addrow(verts, CUBE_SIZE + x, CUBE_SIZE + y, -CUBE_SIZE + z, 1.0f, 1.0f);
                        addrow(verts, CUBE_SIZE + x, CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 0.0f);
                        addrow(verts, CUBE_SIZE + x, CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 0.0f);
                        addrow(verts, -CUBE_SIZE + x, CUBE_SIZE + y, CUBE_SIZE + z, 0.0f, 0.0f);
                        addrow(verts, -CUBE_SIZE + x, CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 1.0f);
                        num_verts += 6;
                    }

                    if (z - 1 < 0 || c->data[x][y][z - 1] == 0) {
                        addrow(verts, -CUBE_SIZE + x, -CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 0.0f);
                        addrow(verts, CUBE_SIZE + x, -CUBE_SIZE + y, -CUBE_SIZE + z, 1.0f, 0.0f);
                        addrow(verts, CUBE_SIZE + x, CUBE_SIZE + y, -CUBE_SIZE + z, 1.0f, 1.0f);
                        addrow(verts, CUBE_SIZE + x, CUBE_SIZE + y, -CUBE_SIZE + z, 1.0f, 1.0f);
                        addrow(verts, -CUBE_SIZE + x, CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 1.0f);
                        addrow(verts, -CUBE_SIZE + x, -CUBE_SIZE + y, -CUBE_SIZE + z, 0.0f, 0.0f);
                        num_verts += 6;
                    }
                    if (z + 1 >= CHUNK_SIZE || c->data[x][y][z + 1] == 0) {
                        addrow(verts, -CUBE_SIZE + x, -CUBE_SIZE + y, CUBE_SIZE + z, 0.0f, 0.0f);
                        addrow(verts, CUBE_SIZE + x, -CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 0.0f);
                        addrow(verts, CUBE_SIZE + x, CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 1.0f);
                        addrow(verts, CUBE_SIZE + x, CUBE_SIZE + y, CUBE_SIZE + z, 1.0f, 1.0f);
                        addrow(verts, -CUBE_SIZE + x, CUBE_SIZE + y, CUBE_SIZE + z, 0.0f, 1.0f);
                        addrow(verts, -CUBE_SIZE + x, -CUBE_SIZE + y, CUBE_SIZE + z, 0.0f, 0.0f);
                        num_verts += 6;
                    }
                }
            }
        }
    }

    c->VBO_len = num_verts;
    fprintf(stderr, "%d", num_verts);
    fprintf(stderr, "\n");

    glGenVertexArrays(1, &c->VAO);
    glGenBuffers(1, &c->VBO);
    glBindVertexArray(c->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, c->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * num_verts, verts.data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vec_deinit(&verts);
}

void render_chunk(chunk *c, mat4 model, unsigned int modelLoc, GLuint tex1, GLuint tex2, vert *verts) {

    setMat4(modelLoc, model);
    glBindVertexArray(c->VAO);
    glDrawArrays(GL_TRIANGLES, 0, c->VBO_len);
}

void set_block(chunk *c, uint16_t x, uint16_t y, uint16_t z, uint8_t state) {
    c->data[x][y][z] = state;
}
uint8_t get_block(chunk *c, uint16_t x, uint16_t y, uint16_t z) {
    return c->data[x][y][z];
}

bool fsave_chunk(chunk *c, char *file_name) {
}
bool fload_chunk(chunk *c, char *file_name) {
}
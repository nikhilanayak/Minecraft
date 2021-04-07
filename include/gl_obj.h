#ifndef GL_VERT_H
#define GL_VERT_H
#include <glad/glad.h>

typedef struct {
	unsigned int VAO;
	unsigned int VBO;
} gl_obj;

void bind_vert(gl_obj* v, const float* data, const int size);

#endif
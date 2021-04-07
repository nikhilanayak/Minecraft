#include "../include/gl_obj.h"

void bind_vert(gl_obj* v, const float* data, const int size){
	glGenVertexArrays(1, &v->VAO);
	glGenBuffers(1, &v->VBO);
	glBindVertexArray(v->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, v->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
    }


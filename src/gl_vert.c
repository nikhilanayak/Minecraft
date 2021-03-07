#include "../include/gl_vert.h"

void delete_vert(vert v) {
	glDeleteVertexArrays(1, &v.VAO);
	glDeleteBuffers(1, &v.VBO);
}
#ifndef TEXTURES_H
#define TEXTURES_H
#include "../include/shader.h"
#include <glad/glad.h>

typedef struct {
	GLuint *data;
} texture_map;

bool load_texture_map(char* dir, char **files) {
}

#endif
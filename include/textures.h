#ifndef TEXTURES_H
#define TEXTURES_H
#include <string.h>

#include "../include/shader.h"
#include <glad/glad.h>

typedef struct {
	GLuint *data;
} texture_map;

bool load_texture_map(char* dir, char **files, int num_files) {
    for(int i = 0; i < num_files; i++){
        char* concat_file = malloc(sizeof(char) * (strlen(dir) + strlen(files[i])));



        free(concat_file);
    }
}

#endif
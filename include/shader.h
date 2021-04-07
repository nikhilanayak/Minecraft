#ifndef SHADER_H
#define SHADER_H
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <glad/glad.h>
#include <stddef.h>
#include <stdio.h>

#include "../include/stb_image.h"
#include "../include/util.h"

GLuint create_shader(const char *vert_path, const char *frag_path);
GLuint load_tex(const char* image_path);

#endif
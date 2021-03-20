#ifndef SHADER_H
#define SHADER_H
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <glad/glad.h>
#include <stddef.h>
#include <stdio.h>

#include "../include/stb_image.h"

GLuint create_shader(const char *vert_path, const char *frag_path);
void setMat4(GLuint location, mat4 mat);
GLuint load_tex(const char* image_path);

#endif
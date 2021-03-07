#ifndef SHADER_H
#define SHADER_H
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <glad/glad.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>

//GLuint create_shader(const char *vertexShaderPath, const char *fragmentShaderPath);
void setMat4(GLuint location, mat4 mat);

void load_tex(GLuint* texture1);

#endif
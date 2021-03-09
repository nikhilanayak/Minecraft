#include "../include/shader.h"

#define ping fprintf(stderr, "ping from %d\n", __LINE__);

char *readFile(const char *path) {

	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		fprintf(stderr, "Could Not Load File With Path: %s\n", path);
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *fileContents = malloc(size + 1);
	fread(fileContents, 1, size, file);
	fclose(file);
	fileContents[size] = 0;
	return fileContents;
}

void setMat4(GLuint location, mat4 mat) {
	glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}

void load_tex(GLuint *texture) {

	int width, height, nrChannels;

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);

	if (data) {
        ping
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        ping
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		fputs(stderr, "Failed To Load Texture\n");
	}
	stbi_image_free(data);
}

GLuint create_shader(const char *vertexShaderPath, const char *fragmentShaderPath) {

	char *vertexShaderSource = readFile(vertexShaderPath);
	char *fragmentShaderSource = readFile(fragmentShaderPath);

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertShader);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragShader);

	free(vertexShaderSource);
	free(fragmentShaderSource);

	GLint status;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if (!status) {
		char buffer[512];
		glGetShaderInfoLog(vertShader, 512, NULL, buffer);
		puts("Vert Shader ERR: \n");
		puts(buffer);
	}
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (!status) {
		char buffer[512];
		glGetShaderInfoLog(fragShader, 512, NULL, buffer);
		puts("Frag Shader ERR: \n");
		puts(buffer);
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		char buffer[512];
		glGetProgramInfoLog(program, 512, NULL, buffer);
		puts("Linking ERR: \n");
		puts(buffer);
	}

	return program;
}
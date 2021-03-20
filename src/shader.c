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
	glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*) mat);
}


GLuint create_shader(const char *vert_path, const char *frag_path) {

	char *vertexShaderSource = readFile(vert_path);
	char *fragmentShaderSource = readFile(frag_path);

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, (const GLchar *const*)&vertexShaderSource, NULL);
	glCompileShader(vertShader);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, (const GLchar *const*)&fragmentShaderSource, NULL);
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
		fputs("Linking ERR: \n", stderr);
		fputs(buffer, stderr);
	}

	return program;
}


GLuint load_tex(const char* image_path) {
	GLuint tex;
	
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(
		true); // tell stb_image.h to flip loaded texture's on the y-axis.

	int width, height, nrChannels;
	unsigned char *data = stbi_load(image_path, &width, &height, &nrChannels, 4);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
					 GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		fputs("Failed To Load Texture\n", stderr);
	}
	stbi_image_free(data);

	return tex;
}

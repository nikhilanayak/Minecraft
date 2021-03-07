#ifndef TEXT_H
#define TEXT_H

#include <freetype2/ft2build.h>
#include <glad/glad.h>
#include <stdio.h>

#include <freetype2/freetype/freetype.h>

GLuint text_VAO, text_VBO;

typedef struct {
	unsigned int TextureID; // ID handle of the glyph texture
	float *Size;			// Size of glyph
	float *Bearing;			// Offset from baseline to left/top of glyph
	unsigned int Advance;	// Offset to advance to next glyph
} character;

character characters[128];

void init_ft() {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could Not Load Freetype\n");
		exit(-1);
	}

	FT_Face face;
	if (FT_New_Face(ft, "/usr/share/fonts/truetype/cascadia-code/CascadiaCode.ttf", 0, &face)) {
		fprintf(stderr, "Could Not Load Font\n");
		exit(-1);
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		fprintf(stderr, "Could Not Load Glyph\n");
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++) {
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			fprintf(stderr, "Could Not Load Glyph\n");
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		vec2 s = {0.0f, 0.0f};
		s[0] = face->glyph->bitmap.width;

		character newchar = {
			.TextureID = texture,
			.Size = (vec2){face->glyph->bitmap.width, face->glyph->bitmap.rows},
			.Bearing = (vec2){face->glyph->bitmap_left, face->glyph->bitmap_top},
			.Advance = face->glyph->advance.x};

		characters[c] = newchar;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &text_VAO);
	glGenBuffers(1, &text_VBO);
	glBindVertexArray(text_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void render_text(GLuint program, char *text, float x, float y, float scale, vec3 color) {
	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "textColor"), color[0], color[1], color[2]);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(text_VAO);

	for (int i = 0, len = strlen(text); i < len; i++) {
		character ch = characters[text[i]];
		float xpos = x + ch.Bearing[0] * scale;
		float ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

		float w = ch.Size[0] * scale;
		float h = ch.Size[1] * scale;

		float vertices[6][4] = {
			{xpos, ypos + h, 0.0f, 0.0f},
			{xpos, ypos, 0.0f, 1.0f},
			{xpos + w, ypos, 1.0f, 1.0f},

			{xpos, ypos + h, 0.0f, 0.0f},
			{xpos + w, ypos, 1.0f, 1.0f},
			{xpos + w, ypos + h, 1.0f, 0.0f}};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
}

#endif

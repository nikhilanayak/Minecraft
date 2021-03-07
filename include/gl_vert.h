#ifndef GL_VERT_H
#define GL_VERT_H

typedef struct {
	unsigned int VAO;
	unsigned int VBO;
} vert;

#define bind_vert(v, data)                                                                          \
	glGenVertexArrays(1, &v.VAO);                                                                    \
	glGenBuffers(1, &v.VBO);                                                                         \
	glBindVertexArray(v.VAO);                                                                        \
	glBindBuffer(GL_ARRAY_BUFFER, v.VBO);                                                            \
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);                               \
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);                   \
	glEnableVertexAttribArray(0);                                                                    \
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float))); \
	glEnableVertexAttribArray(1);

void delete_vert(vert v);

#endif

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>

#include "../include/camera.h"
#include "../include/chunk.h"
#include "../include/gl_vert.h"
#include "../include/load_gl.h"
#include "../include/shader.h"
#include "../include/util.h"
#include "../include/world.h"

#include "../include/datastructures/hashmap.h"
#include "../include/datastructures/vec.h"

//#define RAYGUI_IMPLEMENTATION
//#define RAYGUI_SUPPORT_ICONS
//#include "../include/raygui.h"

//#include "../include/hashmap_test.h"

#define GLT_IMPLEMENTATION
#include "../include/gltext.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/stb_image.h"

#define ping fprintf(stderr, "ping from %d\n", __LINE__);

float delta_time = 0.0f;
float last_frame = 0.0f;

camera c;

const uint8_t RENDER_DISTANCE = 2;

bool keymap[350];
bool mousemap[12];

#define X_POS 0
#define X_MIN 1
#define Y_POS 2
#define Y_MIN 3
#define Z_POS 4
#define Z_MIN 5

float faces[6][30] = {
	{CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
	 CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f,
	 CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f,
	 CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f,
	 CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0.0f, 0.0f,
	 CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f},

	{-CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
	 -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f,
	 -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f,
	 -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f,
	 -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0.0f, 0.0f,
	 -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f},

	{-CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f,
	 CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f,
	 CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
	 CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
	 -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0.0f, 0.0f,
	 -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f},

	{-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f,
	 CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f,
	 CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
	 CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
	 -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0.0f, 0.0f,
	 -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f},

	{-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0.0f, 0.0f,
	 CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
	 CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 1.0f,
	 CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 1.0f,
	 -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0.0f, 1.0f,
	 -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0.0f, 0.0f},

	{-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 0.0f,
	 CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 1.0f, 0.0f,
	 CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f,
	 CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f,
	 -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f,
	 -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 0.0}};

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
						 int mods);
static void error_callback(int error, const char *description);
void mouse_button_callback(GLFWwindow *window, int button, int action,
						   int mods);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void round_vec3(vec3 *vec);
void world_to_screen(vec3 point, mat4 viewMatrix, mat4 projMatrix, vec2 dest, int width, int height);

int main(void) {


	GLFWwindow *window = load_gl("mimecraft", error_callback, key_callback, mouse_callback, mouse_button_callback, true);

	memset(keymap, 0, 350);

	unsigned int dirt_tex, hover_tex;

	dirt_tex = load_tex("assets/dirt.png");
	hover_tex = load_tex("assets/hover.png");

	GLuint cube_VAOs[6];
	GLuint cube_VBOs[6];

	for (int i = 0; i < 6; i++) {
		glGenVertexArrays(1, cube_VAOs[i]);
		glGenBuffers(1, cube_VBOs[1]);
		glBindVertexArray(cube_VAOs[i]);

		glBindBuffer(GL_ARRAY_BUFFER, cube_VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 30, faces[i], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
							  (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	if (!gltInit()) {
		fprintf(stderr, "Failed to initialize glText\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	GLTtext *text1 = gltCreateText();
	GLTtext *current_chunk_text = gltCreateText();

	gltSetText(text1, "Hello World!");

	GLuint program = create_shader("assets/shader.vs", "assets/shader.fs");
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);

	camera_init(&c);
	c.fov = 90.0f;

	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(glm_rad(c.fov), 2560.0 / 1400.0, 0.1f, 1000.0f, projection);
	unsigned int projectionLoc = glGetUniformLocation(program, "projection");
	unsigned int modelLoc = glGetUniformLocation(program, "model");
	unsigned int viewLoc = glGetUniformLocation(program, "view");
	setMat4(projectionLoc, projection);

	world main_world;
	init_world(&main_world);

	c.position[1] = CHUNK_HEIGHT / 2;
	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		delta_time = currentFrame - last_frame;
		last_frame = currentFrame;

		camera_update(&c, keymap, delta_time);

		if (mousemap[GLFW_MOUSE_BUTTON_LEFT]) {
			vec3 sFront = {0.0f, 0.0f, 0.0f};
			for (int i = 0; i < 10; i++) {
				glm_vec3_scale(c.front, i, sFront);

				vec3 newBlock = {c.position[0] + CUBE_SIZE,
								 c.position[1] + CUBE_SIZE,
								 c.position[2] + CUBE_SIZE};

				glm_vec3_add(newBlock, sFront, newBlock);
				round_vec3(&newBlock);
				world_set_block(&main_world, &(i64_coord){.x = newBlock[0], .y = newBlock[1], .z = newBlock[2]}, 0);
			}
		}

		if (mousemap[GLFW_MOUSE_BUTTON_RIGHT]) {
			vec3 sFront = {0.0f, 0.0f, 0.0f};
			for (int i = 9; i >= 0; i--) {
				glm_vec3_scale(c.front, i, sFront);

				vec3 newBlock = {c.position[0] + CUBE_SIZE,
								 c.position[1] + CUBE_SIZE,
								 c.position[2] + CUBE_SIZE};

				glm_vec3_add(newBlock, sFront, newBlock);
				round_vec3(&newBlock);

				if (world_get_block(&main_world, newBlock[0], newBlock[1], newBlock[2] == 0)) {
					world_set_block(&main_world, &(i64_coord){.x = newBlock[0], .y = newBlock[1], .z = newBlock[2]}, 1);
					break;
				}
			}
		}

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, dirt_tex);

		glUseProgram(program);

		mat4 model = GLM_MAT4_IDENTITY_INIT;
		mat4 view = GLM_MAT4_IDENTITY_INIT;

		vec3 dist = {0.0f, 0.0f, -3.0f};
		vec3 center = {0.0f, 0.0f, 0.0f};
		glm_vec3_add(c.position, c.front, center);
		glm_lookat(c.position, center, c.up, view);

		setMat4(viewLoc, view);
		setMat4(modelLoc, model);

		i64_coord chunk_pos = world_pos_to_chunk_pos(&(const i64_coord){.x = c.position[0], c.position[1], c.position[2]});

		for (int x = chunk_pos.x - RENDER_DISTANCE / 2; x <= chunk_pos.x + RENDER_DISTANCE / 2; x++) {
			for (int z = chunk_pos.z - RENDER_DISTANCE / 2; z <= chunk_pos.z + RENDER_DISTANCE / 2; z++) {
				render_chunk_at_chunk_pos(&main_world, x, z, model, modelLoc);
			}
		}

		vec3 sFront = {0.0f, 0.0f, 0.0f};
		for (int i = 0; i < 10; i++) {
			glm_vec3_scale(c.front, i, sFront);

			vec3 newBlock = {c.position[0] + CUBE_SIZE,
							 c.position[1] + CUBE_SIZE,
							 c.position[2] + CUBE_SIZE};

			glm_vec3_add(newBlock, sFront, newBlock);
			round_vec3(&newBlock);
			if (world_get_block(&main_world, newBlock[0], newBlock[1], newBlock[2]) == 1) {
				// draw cube
				glBindTexture(GL_TEXTURE_2D, hover_tex);

				glm_translate_make(model, (vec3){
											  0.0f, 150.0f, 0.0f});
				setMat4(modelLoc, model);

				for (int i = 0; i < 6; i++) {
					glBindVertexArray(cube_VAOs[i]);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				break;
			}
		}

		vec2 screen_pos = {0.0f, 0.0f};
		//world_to_screen((const vec3){1.0f, 1.0f, 1.0f}, view, projection, screen_pos, width, height);
		//printf("%d, %d\n", screen_pos[0], screen_pos[1]);

		gltBeginDraw();

		char current_chunk_str[50];
		i64_coord c_pos = world_pos_to_chunk_pos(&(i64_coord){.x = c.position[0], .y = c.position[1], .z = c.position[2]});
		sprintf(current_chunk_str, "pos: %.3f, %.3f. current chunk: %ld, %ld", c.position[0], c.position[2], c_pos.x, c_pos.z);
		gltSetText(current_chunk_text, current_chunk_str);
		gltColor(1.0f, 1.0f, 1.0f, 0.0f);
		gltDrawText2DAligned(current_chunk_text, 0.0f, 45.0f, 1.0f, GLT_LEFT, GLT_BOTTOM);

		char fps_str[6];
		sprintf(fps_str, "%.3f", (1.0 / delta_time));
		gltSetText(text1, fps_str);
		gltColor(1.0f, 1.0f, 1.0f, 0.0f);
		gltDrawText2DAligned(text1, 0.0f, 25.0f, 1.0f, GLT_LEFT, GLT_BOTTOM);

		gltEndDraw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	deinit_world(&main_world);

	gltDeleteText(current_chunk_text);
	gltDeleteText(text1);

	glDeleteTextures(1, &dirt_tex);

	glDeleteProgram(program);

	glfwDestroyWindow(window);
	glDeleteShader(program);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
						 int mods) {
	keymap[key] = action == 0 ? 0 : 1;
}
static void error_callback(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	camera_mouse_movement(&c, xpos, ypos);
}

void round_vec3(vec3 *vec) {
	(*vec)[0] = (int)(*vec)[0];
	(*vec)[1] = (int)(*vec)[1];
	(*vec)[2] = (int)(*vec)[2];
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
						   int mods) {
	mousemap[button] = action == GLFW_PRESS ? 1 : 0;
}

void world_to_screen(vec3 point, mat4 viewMatrix, mat4 projMatrix, vec2 dest, int width, int height) {
	vec4 clip_space_pos;
	vec4 view_space_pos;

	vec4 point_v4 = {point[0], point[1], point[2], 1.0f};

	glm_mat4_mulv(viewMatrix, point_v4, view_space_pos);
	glm_mat4_mul(projMatrix, view_space_pos, clip_space_pos);

	if (clip_space_pos[3] == 0.0f) {
		printf("clip space is 0\n");
	}

	vec3 ndc_space_pos = {
		clip_space_pos[0] / clip_space_pos[3],
		clip_space_pos[1] / clip_space_pos[3],
		clip_space_pos[2] / clip_space_pos[3]};

	vec2 window_space_pos = {
		((ndc_space_pos[0] + 1.0) / 2.0),
		((ndc_space_pos[1] + 1.0) / 2.0)};

	glm_vec2_mul(window_space_pos, (vec2){width, height}, window_space_pos);

	printf("%.3f, %.3f\n", window_space_pos[0], window_space_pos[1]);

	/*dest[0] = window_space_pos[0];
    dest[1] = window_space_pos[1];*/
}

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>

#include "../include/camera.h"
#include "../include/chunk.h"
#include "../include/gl_obj.h"
#include "../include/load_gl.h"
#include "../include/shader.h"
#include "../include/util.h"
#include "../include/world.h"

#include "../include/datastructures/hashmap.h"
#include "../include/datastructures/vec.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "../include/nuklear.h"
#include "../include/nuklear_glfw_gl3.h"
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define ping fprintf(stderr, "ping from %d\n", __LINE__);

float delta_time = 0.0f;
float last_frame = 0.0f;

bool show_gui = false;

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


static void key_callback(GLFWwindow *window, int key, int scancode, int action,
						 int mods);
static void error_callback(int error, const char *description);
void mouse_button_callback(GLFWwindow *window, int button, int action,
						   int mods);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void round_vec3(vec3 *vec);
void world_to_screen(vec3 point, mat4 viewMatrix, mat4 projMatrix, vec2 dest, int width, int height);
bool world_point_seen(vec3 point, mat4 viewMatrix, mat4 projMatrix, int width, int height);

#define addrow(vec, x, y, z, u, v) \
	vec_push(&vec, x);             \
	vec_push(&vec, y);             \
	vec_push(&vec, z);             \
	vec_push(&vec, u);             \
	vec_push(&vec, v);

int main(void) {

	GLFWwindow *window = load_gl("mimecraft", error_callback, key_callback, mouse_callback, mouse_button_callback, true);

	memset(keymap, 0, 350);

	unsigned int dirt_tex, hover_tex;

	dirt_tex = load_tex("assets/dirt.png");
	hover_tex = load_tex("assets/hover.png");

	GLuint program = create_shader("assets/shader.vs", "assets/shader.fs");
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	camera_init(&c, mode->width, mode->height);
	c.fov = 90.0f;

	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(glm_rad(c.fov), 2560.0 / 1400.0, 0.1f, 1000.0f, projection);
	unsigned int projectionLoc = glGetUniformLocation(program, "projection");
	unsigned int modelLoc = glGetUniformLocation(program, "model");
	unsigned int viewLoc = glGetUniformLocation(program, "view");
	set_mat4(projectionLoc, projection);

	world main_world;
	init_world(&main_world);

	c.position[1] = 0;

	struct nk_glfw nk_glfw_obj = {0};
	struct nk_context *ctx = nk_glfw3_init(&nk_glfw_obj, window, NK_GLFW3_DEFAULT);
	struct nk_colorf bg;

	struct nk_font_atlas *atlas;
	nk_glfw3_font_stash_begin(&nk_glfw_obj, &atlas);
	nk_glfw3_font_stash_end(&nk_glfw_obj);
	bg.r = 1.00f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 0.0f;

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		delta_time = currentFrame - last_frame;
		last_frame = currentFrame;

		if (!show_gui) {
			camera_update(&c, keymap, delta_time);
		}

		if (keymap[GLFW_KEY_ESCAPE]) {
			show_gui = true;
			window_show_mouse(window);
		}
		if (keymap[GLFW_KEY_E]) {
			show_gui = false;
			window_hide_mouse(window);
		}

		if (!show_gui && mousemap[GLFW_MOUSE_BUTTON_LEFT]) {
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

		if (!show_gui && mousemap[GLFW_MOUSE_BUTTON_RIGHT]) {
			vec3 sFront = {0.0f, 0.0f, 0.0f};
			for (int i = 9; i >= 0; i--) {
				glm_vec3_scale(c.front, i, sFront);

				vec3 newBlock = {c.position[0] + CUBE_SIZE,
								 c.position[1] + CUBE_SIZE,
								 c.position[2] + CUBE_SIZE};

				glm_vec3_add(newBlock, sFront, newBlock);
				round_vec3(&newBlock);

				if (world_get_block(&main_world, newBlock[0], newBlock[1], newBlock[2]) == 0) {
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

		set_mat4(viewLoc, view);
		set_mat4(modelLoc, model);

		i64_coord chunk_pos = world_pos_to_chunk_pos(&(const i64_coord){.x = c.position[0], c.position[1], c.position[2]});

		for (int x = chunk_pos.x - RENDER_DISTANCE / 2; x <= chunk_pos.x + RENDER_DISTANCE / 2; x++) {
			for (int z = chunk_pos.z - RENDER_DISTANCE / 2; z <= chunk_pos.z + RENDER_DISTANCE / 2; z++) {
				render_chunk_at_chunk_pos(&main_world, x, z, model, modelLoc);
			}
		}

		//nk_glfw3_render(&nk_glfw_obj, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
		glEnable(GL_DEPTH_TEST);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	deinit_world(&main_world);

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
	if (!show_gui) {
		camera_mouse_movement(&c, xpos, ypos);
	}
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
	glm_mat4_mulv(projMatrix, view_space_pos, clip_space_pos);

	if (clip_space_pos[3] == 0.0f) {
		printf("%.3f", glfwGetTime());
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

	dest[0] = window_space_pos[0];
	dest[1] = window_space_pos[1];
}

bool world_point_seen(vec3 point, mat4 viewMatrix, mat4 projMatrix, int width, int height) {
	vec2 dest;
	world_to_screen(point, viewMatrix, projMatrix, dest, width, height);
	return !((dest[0] < 0 || dest[0] > width) && (dest[1] < 0 || dest[1] > height));
}

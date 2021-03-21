
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>

#include "../include/chunk.h"
#include "../include/gl_vert.h"
#include "../include/shader.h"
#include "../include/util.h"
#include "../include/load_gl.h"
#include "../include/world.h"

#include "../include/datastructures/hashmap.h"
#include "../include/datastructures/vec.h"

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

vec3 player_pos = {0.0f, CHUNK_HEIGHT, 0.0f};
vec3 camera_front = {0.0f, 0.0f, -1.0f};
vec3 camera_front_without_y = {0.0f, 0.0f, -1.0f};
vec3 camera_up = {0.0f, 1.0f, 0.0f};

float delta_time = 0.0f; // time between current frame and last frame
float last_frame = 0.0f;

bool first_mouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float last_x = 800.0f / 2.0;
float last_y = 600.0 / 2.0;
float fov = 45.0f;

const uint8_t RENDER_DISTANCE = 10;

bool keymap[350];
bool mousemap[12];

#define X_POS 0
#define X_MIN 1
#define Y_POS 2
#define Y_MIN 3
#define Z_POS 4
#define Z_MIN 5

float faces[6][30] = {
	{CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f, CUBE_SIZE,
	 CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f, CUBE_SIZE, -CUBE_SIZE,
	 -CUBE_SIZE, 0.0f, 1.0f, CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE,
	 0.0f, 1.0f, CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0.0f,
	 0.0f, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f},

	{-CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f, -CUBE_SIZE,
	 CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f, -CUBE_SIZE, -CUBE_SIZE,
	 -CUBE_SIZE, 0.0f, 1.0f, -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE,
	 0.0f, 1.0f, -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0.0f,
	 0.0f, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f},

	{-CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f, CUBE_SIZE,
	 CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f, CUBE_SIZE, CUBE_SIZE,
	 CUBE_SIZE, 1.0f, 0.0f, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE,
	 1.0f, 0.0f, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0.0f,
	 0.0f, -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f},

	{-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f, CUBE_SIZE,
	 -CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f, CUBE_SIZE, -CUBE_SIZE,
	 CUBE_SIZE, 1.0f, 0.0f, CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE,
	 1.0f, 0.0f, -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0.0f,
	 0.0f, -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f},

	{-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0.0f, 0.0f, CUBE_SIZE,
	 -CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f, CUBE_SIZE, CUBE_SIZE,
	 CUBE_SIZE, 1.0f, 1.0f, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE,
	 1.0f, 1.0f, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0.0f,
	 1.0f, -CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, 0.0f, 0.0f},

	{-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 0.0f, CUBE_SIZE,
	 -CUBE_SIZE, -CUBE_SIZE, 1.0f, 0.0f, CUBE_SIZE, CUBE_SIZE,
	 -CUBE_SIZE, 1.0f, 1.0f, CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE,
	 1.0f, 1.0f, -CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0.0f,
	 1.0f, -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0.0f, 0.0}};

#define CUBE_SIZE 0.5f
float top_face[] = {-CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f,
					CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f,
					CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
					CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
					-CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0.0f, 0.0f,
					-CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f};

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
						 int mods);
static void error_callback(int error, const char *description);
void mouse_button_callback(GLFWwindow *window, int button, int action,
						   int mods);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void round_vec3(vec3 *vec);
void player_movement(float delta_time);

float xRot = 0.0f;
float yRot = 0.0f;

float time_since_last_block_update = 0.0f;

int main(void) {

    GLFWwindow* window = load_gl("mimecraft", error_callback, key_callback, mouse_callback, mouse_button_callback, true);
	

	memset(keymap, 0, 350);


	unsigned int texture1, texture2;
	unsigned char *data;
	int width, height, nrChannels;

	texture1 = load_tex("assets/dirt.png");

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

	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(glm_rad(45.0f), 2560.0 / 1400.0, 0.1f, 1000.0f, projection);
	unsigned int projectionLoc = glGetUniformLocation(program, "projection");
	unsigned int modelLoc = glGetUniformLocation(program, "model");
	unsigned int viewLoc = glGetUniformLocation(program, "view");
	setMat4(projectionLoc, projection);

	world main_world;
	init_world(&main_world);

	bool draw = false;

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		delta_time = currentFrame - last_frame;
		last_frame = currentFrame;

		player_movement(delta_time);

		if (mousemap[GLFW_MOUSE_BUTTON_LEFT]) {
			time_since_last_block_update = 0;
			vec3 sFront = {0.0f, 0.0f, 0.0f};
			for (int i = 0; i < 10; i++) {
				glm_vec3_scale(camera_front, i, sFront);

				vec3 newBlock = {player_pos[0] + CUBE_SIZE,
								 player_pos[1] + CUBE_SIZE,
								 player_pos[2] + CUBE_SIZE};

				glm_vec3_add(newBlock, sFront, newBlock);
				round_vec3(&newBlock);
                world_set_block(&main_world, &(i64_coord){.x = newBlock[0], .y = newBlock[1], .z = newBlock[2]}, 0);
			}
		}

		if (mousemap[GLFW_MOUSE_BUTTON_RIGHT]) {
			time_since_last_block_update = 0;
			vec3 sFront = {0.0f, 0.0f, 0.0f};
			for (int i = 9; i >= 0; i--) {
				glm_vec3_scale(camera_front, i, sFront);

				vec3 newBlock = {player_pos[0] + CUBE_SIZE,
								 player_pos[1] + CUBE_SIZE,
								 player_pos[2] + CUBE_SIZE};

				glm_vec3_add(newBlock, sFront, newBlock);
				round_vec3(&newBlock);

                world_set_block(&main_world, &(i64_coord){.x = newBlock[0], .y = newBlock[1], .z = newBlock[2]}, 1);
			}
		}

		if (keymap[GLFW_KEY_0]) {
			draw = true;
		}
		if (keymap[GLFW_KEY_9]) {
			draw = false;
		}

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glUseProgram(program);

		mat4 model = GLM_MAT4_IDENTITY_INIT;
		mat4 view = GLM_MAT4_IDENTITY_INIT;

		vec3 dist = {0.0f, 0.0f, -3.0f};
		vec3 center = {0.0f, 0.0f, 0.0f};
		glm_vec3_add(player_pos, camera_front, center);
		glm_lookat(player_pos, center, camera_up, view);

		setMat4(viewLoc, view);
		setMat4(modelLoc, model);

		
        i64_coord chunk_pos = world_pos_to_chunk_pos(&(const i64_coord){.x = player_pos[0], player_pos[1], player_pos[2]});

        for(int x = chunk_pos.x - RENDER_DISTANCE/2; x <= chunk_pos.x + RENDER_DISTANCE/2; x++){
            for(int z = chunk_pos.z - RENDER_DISTANCE/2; z <= chunk_pos.z + RENDER_DISTANCE/2; z++){
                render_chunk_at_chunk_pos(&main_world, x, z, model, modelLoc);
            }
        }

//		render_chunk_at_world_pos(&main_world, & (i64_coord){player_pos[0], player_pos[1], player_pos[2]}, model, modelLoc);

		gltBeginDraw();

		char current_chunk_str[50];
		i64_coord c_pos = world_pos_to_chunk_pos(&(i64_coord){.x = player_pos[0], .y = player_pos[1], .z = player_pos[2]});
		sprintf(current_chunk_str, "pos: %.3f, %.3f. current chunk: %ld, %ld", player_pos[0], player_pos[2], c_pos.x, c_pos.z);
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
	if (first_mouse) {
		last_x = xpos;
		last_y = ypos;
		first_mouse = false;
	}

	float xoffset = xpos - last_x;
	float yoffset =
		last_y - ypos; // reversed since y-coordinates go from bottom to top
	last_x = xpos;
	last_y = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


	vec3 front = {0.0f, 0.0f, 0.0f};
	front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
	front[1] = sin(glm_rad(pitch));
	front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
	glm_normalize_to(front, camera_front);



    glm_vec3_zero(front);
	front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = 0;
	front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
	glm_normalize_to(front, camera_front_without_y);


}

void round_vec3(vec3 *vec) {
	(*vec)[0] = (int)(*vec)[0];
	(*vec)[1] = (int)(*vec)[1];
	(*vec)[2] = (int)(*vec)[2];
}

void player_movement(float delta_time) {
	float camera_speed = 2.5 * delta_time;
	float old_x_angle = camera_front[1];

	//camera_front[1] = 0.0f;
    //printf("%.3f, %.3f, %.3f\n", camera_front[0], camera_front[1], camera_front[2]);

    vec3 altered_camera_front = {0.0f, 0.0f, 0.0f};
    glm_vec3_copy(camera_front_without_y, altered_camera_front);

    //altered_camera_front[1] = 0.0f;
    //altered_camera_front[2] = -1.0f;

	if (keymap[GLFW_KEY_SPACE]) {
		vec3 speed_up = {0.0f, 0.0f, 0.0f};
		glm_vec3_scale(camera_up, camera_speed, speed_up);
		glm_vec3_add(player_pos, speed_up, player_pos);
	}
	if (keymap[GLFW_KEY_LEFT_SHIFT]) {
		vec3 speed_up = {0.0f, 0.0f, 0.0f};
		glm_vec3_scale(camera_up, -camera_speed, speed_up);
		glm_vec3_add(player_pos, speed_up, player_pos);
	}
	if (keymap[GLFW_KEY_W]) {
		vec3 speed_front = {0.0f, 0.0f, 0.0f};
        glm_normalize(speed_front);
		glm_vec3_scale(altered_camera_front, camera_speed, speed_front);
		glm_vec3_add(player_pos, speed_front, player_pos);

	}
	if (keymap[GLFW_KEY_S]) {
		vec3 speed_front = {0.0f, 0.0f, 0.0f};
		glm_vec3_scale(altered_camera_front, camera_speed, speed_front);
		glm_vec3_sub(player_pos, speed_front, player_pos);
	}
	if (keymap[GLFW_KEY_A]) {
		vec3 normalized = {0.0f, 0.0f, 0.0f};
		glm_cross(altered_camera_front, camera_up, normalized);
		glm_normalize(normalized);
		glm_vec3_scale(normalized, camera_speed, normalized);

		glm_vec3_sub(player_pos, normalized, player_pos);
	}
	if (keymap[GLFW_KEY_D]) {
		vec3 normalized = {0.0f, 0.0f, 0.0f};
		glm_cross(altered_camera_front, camera_up, normalized);
		glm_normalize(normalized);
		glm_vec3_scale(normalized, camera_speed, normalized);

		glm_vec3_add(player_pos, normalized, player_pos);
	}
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
						   int mods) {
	mousemap[button] = action == GLFW_PRESS ? 1 : 0;
}



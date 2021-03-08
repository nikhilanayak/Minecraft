
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>

#include "../include/util.h"
#include "../include/chunk.h"
#include "../include/gl_vert.h"
#include "../include/shader.h"
#include "../include/vec.h"

#define GLT_IMPLEMENTATION
#include "../include/gltext.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	vec3 position;
	vec3 rotation;
} block_;

#define ping fprintf(stderr, "ping from %d\n", __LINE__);

vec3 cameraPos = {0.0f, CHUNK_HEIGHT, 0.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

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

#define CUBE_SIZE 0.5f
float top_face[] = {
	-CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f,
	CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 1.0f, 1.0f,
	CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
	CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 1.0f, 0.0f,
	-CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, 0.0f, 0.0f,
	-CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE, 0.0f, 1.0f};

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void error_callback(int error, const char *description);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void round_vec3(vec3 *vec);
void movementFunction(float deltaTime);

float xRot = 0.0f;
float yRot = 0.0f;

float time_since_last_block_update = 0.0f;

int main(void) {
	GLFWwindow *window;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//	window = glfwCreateWindow(2560, 1440, "Simple example", glfwGetPrimaryMonitor(), NULL);
	window = glfwCreateWindow(2560, 1440, "Simple example", NULL, NULL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Could Not Load GLAD\n");
		exit(1);
	}

	puts("GPU: ");
	puts(glGetString(GL_RENDERER));

	//    puts()

	glEnable(GL_DEPTH_TEST);

	glfwSwapInterval(0);

	memset(keymap, 0, 350);

	vert faces_vert[6];
	for (int i = 0; i < 6; i++) {
		bind_vert(faces_vert[i], faces[i]);
	}

	unsigned int texture1, texture2;
	unsigned char *data;
	int width, height, nrChannels;

	//	texture1 = load_tex("assets/container.jpg");
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		fputs(stderr, "Failed To Load Texture\n");
	}
	stbi_image_free(data);

	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	data = stbi_load("assets/dirt.png", &width, &height, &nrChannels, 0);
	if (data) {
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		fputs(stderr, "Failed To Load Texture\n");
	}
	stbi_image_free(data);

	if (!gltInit()) {
		fprintf(stderr, "Failed to initialize glText\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	GLTtext *text1 = gltCreateText();
	gltSetText(text1, "Hello World!");

	GLuint program = create_shader("./assets/shader.vs", "./assets/shader.fs");
	glUseProgram(program);

	glUniform1i(glGetUniformLocation(program, "tex"), 0);

	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(glm_rad(45.0f), 2560.0 / 1400.0, 0.1f, 1000.0f, projection);
	unsigned int projectionLoc = glGetUniformLocation(program, "projection");
	unsigned int modelLoc = glGetUniformLocation(program, "model");
	unsigned int viewLoc = glGetUniformLocation(program, "view");
	setMat4(projectionLoc, projection);

	chunk test_chunk;
	chunk_init(&test_chunk);

#define off 1
	for (int x = 0; x < CHUNK_SIZE; x += off) {
		for (int y = 0; y < CHUNK_HEIGHT; y += off) {
			for (int z = 0; z < CHUNK_SIZE; z += off) {
				test_chunk.data[x][y][z] = 1;
			}
		}
	}
	build_mesh(&test_chunk);

	bool draw = false;

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		movementFunction(deltaTime);

		if (mousemap[GLFW_MOUSE_BUTTON_LEFT]) {
            time_since_last_block_update = 0;
			vec3 sFront = {0.0f, 0.0f, 0.0f};
			for (int i = 0; i < 10; i++) {
				glm_vec3_scale(cameraFront, i, sFront);

				vec3 newBlock = {cameraPos[0] + CUBE_SIZE, cameraPos[1] + CUBE_SIZE, cameraPos[2] + CUBE_SIZE};

				glm_vec3_add(newBlock, sFront, newBlock);
				round_vec3(&newBlock);
				//			vector_add(&blocks, newBlock);
				if (test_chunk.data[(int)newBlock[0]][(int)newBlock[1]][(int)newBlock[2]] == 1) {
					test_chunk.data[(int)newBlock[0]][(int)newBlock[1]][(int)newBlock[2]] = 0;
					build_mesh(&test_chunk);
					break;
				}
			}
		}

		if (mousemap[GLFW_MOUSE_BUTTON_RIGHT]) {
            time_since_last_block_update = 0;
			vec3 sFront = {0.0f, 0.0f, 0.0f};
			for (int i = 9; i >= 0; i--) {
				glm_vec3_scale(cameraFront, i, sFront);

				vec3 newBlock = {cameraPos[0] + CUBE_SIZE, cameraPos[1] + CUBE_SIZE, cameraPos[2] + CUBE_SIZE};

				glm_vec3_add(newBlock, sFront, newBlock);
				round_vec3(&newBlock);
				//			vector_add(&blocks, newBlock);
				if (test_chunk.data[(int)newBlock[0]][(int)newBlock[1]][(int)newBlock[2]] == 0) {
					test_chunk.data[(int)newBlock[0]][(int)newBlock[1]][(int)newBlock[2]] = 1;
					build_mesh(&test_chunk);
					break;
				}
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
		glBindTexture(GL_TEXTURE_2D, texture2);

		glUseProgram(program);

		mat4 model = GLM_MAT4_IDENTITY_INIT;
		mat4 view = GLM_MAT4_IDENTITY_INIT;

		vec3 dist = {0.0f, 0.0f, -3.0f};
		vec3 center = {0.0f, 0.0f, 0.0f};
		glm_vec3_add(cameraPos, cameraFront, center);
		glm_lookat(cameraPos, center, cameraUp, view);

		setMat4(viewLoc, view);
		setMat4(modelLoc, model);

		if (!draw) {
			glBindVertexArray(faces_vert->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		} else {
			for (int x = 0; x < 10; x++) {
				for (int y = 0; y < 10; y++) {
					glm_translate_make(model, (vec3){CHUNK_SIZE * x, 0, CHUNK_SIZE * y});
					setMat4(modelLoc, model);
					render_chunk(&test_chunk, model, modelLoc, texture1, texture2, faces_vert);
				}
			}
		}

		char fps_str[6];
		sprintf(fps_str, "%.3f", (1.0 / deltaTime));

		gltSetText(text1, fps_str);

		gltBeginDraw();
		gltColor(1.0f, 1.0f, 1.0f, 0.0f);
		gltDrawText2D(text1, 0.0f, 0.0f, 1.0f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(program);

	glfwDestroyWindow(window);
	glDeleteShader(program);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	keymap[key] = action == 0 ? 0 : 1;
}
static void error_callback(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

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
	glm_normalize_to(front, cameraFront);
}

void round_vec3(vec3 *vec) {
	(*vec)[0] = (int)(*vec)[0];
	(*vec)[1] = (int)(*vec)[1];
	(*vec)[2] = (int)(*vec)[2];
}

void movementFunction(float deltaTime) {
	float cameraSpeed = 2.5 * deltaTime;
	float oldXAngle = cameraFront[1];

	cameraFront[1] = 0.0f;

	if (keymap[GLFW_KEY_SPACE]) {
		vec3 speedUp = {0.0f, 0.0f, 0.0f};
		glm_vec3_scale(cameraUp, cameraSpeed, speedUp);
		glm_vec3_add(cameraPos, speedUp, cameraPos);
	}
	if (keymap[GLFW_KEY_LEFT_SHIFT]) {
		vec3 speedUp = {0.0f, 0.0f, 0.0f};
		glm_vec3_scale(cameraUp, -cameraSpeed, speedUp);
		glm_vec3_add(cameraPos, speedUp, cameraPos);
	}
	if (keymap[GLFW_KEY_W]) {
		vec3 speedFront = {0.0f, 0.0f, 0.0f};
		glm_vec3_scale(cameraFront, cameraSpeed, speedFront);
		glm_vec3_add(cameraPos, speedFront, cameraPos);
	}
	if (keymap[GLFW_KEY_S]) {
		vec3 speedFront = {0.0f, 0.0f, 0.0f};
		glm_vec3_scale(cameraFront, cameraSpeed, speedFront);
		glm_vec3_sub(cameraPos, speedFront, cameraPos);
	}
	if (keymap[GLFW_KEY_A]) {
		vec3 normalized = {0.0f, 0.0f, 0.0f};
		glm_cross(cameraFront, cameraUp, normalized);
		glm_normalize(normalized);
		glm_vec3_scale(normalized, cameraSpeed, normalized);

		glm_vec3_sub(cameraPos, normalized, cameraPos);
	}
	if (keymap[GLFW_KEY_D]) {
		vec3 normalized = {0.0f, 0.0f, 0.0f};
		glm_cross(cameraFront, cameraUp, normalized);
		glm_normalize(normalized);
		glm_vec3_scale(normalized, cameraSpeed, normalized);

		glm_vec3_add(cameraPos, normalized, cameraPos);
	}
	cameraFront[1] = oldXAngle;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	mousemap[button] = action == GLFW_PRESS ? 1 : 0;
}
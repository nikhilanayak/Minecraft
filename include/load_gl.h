#ifndef LOAD_GL_H
#define LOAD_GL_H

#include <stdbool.h>
#include <stdio.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

GLFWwindow *load_gl(
	char *title,
    GLFWerrorfun error_callback,
	void (*key_callback)(GLFWwindow *window, int key, int scancode, int action, int mods),
	void (*mouse_callback)(GLFWwindow *window, double xpos, double ypos),
	void (*mouse_button_callback)(GLFWwindow *window, int button, int action, int mods),
    bool vsync
    ) {

	if (!glfwInit()) {
		fputs("Could not load GLFW\n", stderr);
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

//	GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, title, glfwGetPrimaryMonitor(), NULL);
	GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, title, NULL, NULL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!window) {
		glfwTerminate();
		fputs("Could Not Create Window\n", stderr);
		exit(1);
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

	glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);


	glfwSwapInterval(vsync);

	return window;
}


void window_show_mouse(GLFWwindow* w){

	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void window_hide_mouse(GLFWwindow* w){
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

#endif
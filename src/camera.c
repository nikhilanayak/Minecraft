#include "../include/camera.h"

void camera_init(camera *c, int width, int height) {
	glm_vec3_zero(c->position);

	c->front[0] = c->front[1] = 0.0f;
	c->front[2] = -1.0f;

	glm_vec3_copy(c->front, c->front_no_y);

	c->up[0] = c->up[2] = 0.0f;
	c->up[1] = 1.0f;

	c->yaw = -90.0f;
	c->pitch = 0.0f;

	// TODO: Change this to resolution
	c->last_x = (float)width / 2.0;
	c->last_y = (float)height / 2.0;

	c->fov = 45.0f;

	c->first_mouse;
}

void camera_update(camera* c, bool* keymap, float delta_time){
	float camera_speed = 10.0 * delta_time;
	float old_x_angle = c->front[1];

	//camera_front[1] = 0.0f;
    //printf("%.3f, %.3f, %.3f\n", camera_front[0], camera_front[1], camera_front[2]);

    vec3 altered_camera_front = {0.0f, 0.0f, 0.0f};
    glm_vec3_copy(c->front_no_y, altered_camera_front);

    //altered_camera_front[1] = 0.0f;
    //altered_camera_front[2] = -1.0f;

	if (keymap[GLFW_KEY_SPACE]) {
		vec3 speed_up = {0.0f, 0.0f, 0.0f};
		glm_vec3_scale(c->up, camera_speed, speed_up);
		glm_vec3_add(c->position, speed_up, c->position);
	}
	if (keymap[GLFW_KEY_LEFT_SHIFT]) {
		vec3 speed_up = {0.0f, 0.0f, 0.0f};
		glm_vec3_scale(c->up, -camera_speed, speed_up);
		glm_vec3_add(c->position, speed_up, c->position);
	}
	if (keymap[GLFW_KEY_W]) {
		vec3 speed_front = {0.0f, 0.0f, 0.0f};
        glm_normalize(speed_front);
		glm_vec3_scale(altered_camera_front, camera_speed, speed_front);
		glm_vec3_add(c->position, speed_front, c->position);

	}
	if (keymap[GLFW_KEY_S]) {
		vec3 speed_front = {0.0f, 0.0f, 0.0f};
		glm_vec3_scale(altered_camera_front, camera_speed, speed_front);
		glm_vec3_sub(c->position, speed_front, c->position);
	}
	if (keymap[GLFW_KEY_A]) {
		vec3 normalized = {0.0f, 0.0f, 0.0f};
		glm_cross(altered_camera_front, c->up, normalized);
		glm_normalize(normalized);
		glm_vec3_scale(normalized, camera_speed, normalized);

		glm_vec3_sub(c->position, normalized, c->position);
	}
	if (keymap[GLFW_KEY_D]) {
		vec3 normalized = {0.0f, 0.0f, 0.0f};
		glm_cross(altered_camera_front, c->up, normalized);
		glm_normalize(normalized);
		glm_vec3_scale(normalized, camera_speed, normalized);

		glm_vec3_add(c->position, normalized, c->position);
	}
}

void camera_mouse_movement(camera* c, double xpos, double ypos){
if (c->first_mouse) {
		c->last_x = xpos;
		c->last_y = ypos;
		c->first_mouse = false;
	}

	float xoffset = xpos - c->last_x;
	float yoffset =
		c->last_y - ypos; // reversed since y-coordinates go from bottom to top
	c->last_x = xpos;
	c->last_y = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	c->yaw += xoffset;
	c->pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (c->pitch > 89.0f)
		c->pitch = 89.0f;
	if (c->pitch < -89.0f)
		c->pitch = -89.0f;


	vec3 front = {0.0f, 0.0f, 0.0f};
	front[0] = cos(glm_rad(c->yaw)) * cos(glm_rad(c->pitch));
	front[1] = sin(glm_rad(c->pitch));
	front[2] = sin(glm_rad(c->yaw)) * cos(glm_rad(c->pitch));
	glm_normalize_to(front, c->front);



    glm_vec3_zero(front);
	front[0] = cos(glm_rad(c->yaw)) * cos(glm_rad(c->pitch));
    front[1] = 0;
	front[2] = sin(glm_rad(c->yaw)) * cos(glm_rad(c->pitch));
	glm_normalize_to(front, c->front_no_y);

}
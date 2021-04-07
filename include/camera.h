#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <cglm/vec3.h>

#include <GLFW/glfw3.h>

typedef struct{
    vec3 position;
    vec3 front;
    vec3 front_no_y;
    vec3 up;

    float pitch;
    float yaw;

    float last_x;
    float last_y;

    float fov;
    bool first_mouse;

} camera;

void camera_init(camera* c, int width, int height);
void camera_update(camera* c, bool* keymap, float delta_time);

#endif
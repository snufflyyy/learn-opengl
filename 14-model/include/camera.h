#pragma once

#include <cglm/cglm.h>

typedef struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;

    mat4 projection;
    mat4 view;

    float fov;
    float move_speed;

    bool first_mouse;
    float mouse_sensitivity;
    float last_mouse_x;
    float last_mouse_y;

    float yaw;
    float pitch;
} Camera;

Camera camera_create(vec3 camera_position, int viewport_width, int viewport_height, float fov, float move_speed, float mouse_sensitivity);
void camera_resize_projection(Camera* camera, int viewport_width, int viewport_height);
void camera_move_forward(Camera* camera, float delta_time);
void camera_move_backward(Camera* camera, float delta_time);
void camera_move_left(Camera* camera, float delta_time);
void camera_move_right(Camera* camera, float delta_time);
void camera_mouse_movement(Camera* camera, float mouse_x, float mouse_y);
void camera_update(Camera* camera);
void camera_destroy(Camera* camera);

#include <camera.h>

#include <stdbool.h>
#include <math.h>

#include <cglm/cglm.h>

Camera camera_create(vec3 camera_position, int viewport_width, int viewport_height, float fov, float move_speed, float mouse_sensitivity) {
    Camera camera = {0};

    glm_vec3_copy(camera_position, camera.position);
    glm_vec3_copy((vec3) {0.0f, 0.0f, -1.0f}, camera.front);
    glm_vec3_copy((vec3) {0.0f, 1.0f, 0.0f}, camera.up);

    mat4 view_matrix = GLM_MAT4_IDENTITY_INIT;

    camera.fov = fov;
    glm_perspective(glm_rad(fov), (float) viewport_width / (float) viewport_height, 0.01f, 100.0f, camera.projection);

    camera.move_speed = move_speed;

    camera.first_mouse = true;
    camera.mouse_sensitivity = mouse_sensitivity;

    return camera;
}

void camera_resize_projection(Camera* camera, int viewport_width, int viewport_height) {
    glm_perspective(glm_rad(camera->fov), (float) viewport_width / (float) viewport_height, 0.01f, 100.0f, camera->projection);
}

void camera_move_forward(Camera* camera, float delta_time) {
    float camera_move_speed = camera->move_speed * delta_time;

    vec3 forward;
    glm_vec3_scale(camera->front, camera_move_speed, forward);
    glm_vec3_add(camera->position, forward, camera->position);
}

void camera_move_backward(Camera* camera, float delta_time) {
    float camera_move_speed = camera->move_speed * delta_time;

    vec3 backward;
    glm_vec3_scale(camera->front, camera_move_speed, backward);
    glm_vec3_sub(camera->position, backward, camera->position);
}

void camera_move_left(Camera* camera, float delta_time) {
    float camera_move_speed = camera->move_speed * delta_time;

    vec3 left;
    glm_vec3_crossn(camera->front, camera->up, left);
    glm_vec3_scale(left, camera_move_speed, left);
    glm_vec3_sub(camera->position, left, camera->position);
}

void camera_move_right(Camera* camera, float delta_time) {
    float camera_move_speed = camera->move_speed * delta_time;

    vec3 right;
    glm_vec3_crossn(camera->front, camera->up, right);
    glm_vec3_scale(right, camera_move_speed, right);
    glm_vec3_add(camera->position, right, camera->position);
}

void camera_mouse_movement(Camera* camera, float mouse_x, float mouse_y) {
    if (camera->first_mouse) {
        camera->last_mouse_x = mouse_x;
        camera->last_mouse_y = mouse_y;
        camera->first_mouse = false;
        return;
    }

    float x_offset = mouse_x - camera->last_mouse_x;
    float y_offset = camera->last_mouse_y - mouse_y;
    camera->last_mouse_x = mouse_x;
    camera->last_mouse_y = mouse_y;

    x_offset *= camera->mouse_sensitivity;
    y_offset *= camera->mouse_sensitivity;

    camera->yaw += x_offset;
    camera->pitch += y_offset;

    if (camera->pitch > 89.0f) { camera->pitch = 89.0f; }
    if (camera->pitch < -89.0f) { camera->pitch = -89.0f; }

    vec3 direction = {
        cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)),
        sin(glm_rad(camera->pitch)),
        sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch))
    };
    glm_normalize(direction);
    glm_vec3_copy(direction, camera->front);
}

void camera_update(Camera* camera) {
    glm_mat4_identity(camera->view);
    vec3 target;
    glm_vec3_add(camera->position, camera->front, target);
    glm_lookat(camera->position, target, camera->up, camera->view);
}

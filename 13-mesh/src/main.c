#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "mesh.h"
#include "window.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"

Vertex cube_vertices[] = {
    {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f, 1.0f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f,-1.0f}, {1.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f,-1.0f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f,-1.0f}, {0.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f,-1.0f}, {1.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f, 0.0f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f, 0.0f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f, 0.0f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f, 0.0f}, {0.0f, 1.0f}},

    {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f, 0.0f}, {1.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f, 0.0f}, {0.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f, 0.0f}, {1.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},

    {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f, 0.0f}, {0.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f, 0.0f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f, 0.0f}, {0.0f, 0.0f}},
};

GLuint cube_indices[] = {
    0, 1, 2,   2, 3, 0,
    4, 5, 6,   6, 7, 4,
    8, 9,10,  10,11, 8,
   12,13,14,  14,15,12,
   16,17,18,  18,19,16,
   20,21,22,  22,23,20
};

int main() {
    Window* window = window_create(1280, 720, "learn-opengl");

    Camera camera = camera_create((vec3) {0.0f, 0.0f, 3.0f}, window_get_width(window), window_get_height(window), 60.0f, 5.0f, 0.1f);

    Shader shader = shader_create("../shaders/lighting.vert", "../shaders/lighting.frag");

    Texture diffuse_texture = texture_create("../assets/textures/container.png");
    Texture specular_texture = texture_create("../assets/textures/container_specular.png");
    Mesh cube_mesh = mesh_create(cube_vertices, sizeof(cube_vertices) / sizeof(Vertex), cube_indices, sizeof(cube_indices) / sizeof(GLuint), diffuse_texture, specular_texture);

    mat4 object_model = GLM_MAT4_IDENTITY_INIT;

    GLuint model_uniform_location = glGetUniformLocation(shader.program, "model");
    GLuint view_uniform_location = glGetUniformLocation(shader.program, "view");
    GLuint projection_uniform_location = glGetUniformLocation(shader.program, "projection");
    GLuint view_position_uniform_location = glGetUniformLocation(shader.program, "view_position");

    shader_bind(shader);
    shader_set_int_uniform(glGetUniformLocation(shader.program, "material.diffuse"), 0);
    shader_set_int_uniform(glGetUniformLocation(shader.program, "material.specular"), 1);
    shader_set_float_uniform(glGetUniformLocation(shader.program, "material.shininess"), 32.0f);

    shader_set_vec3_uniform(glGetUniformLocation(shader.program, "directional_light.direction"), (vec3) {-1.0f, -1.0f, -1.0f});
    shader_set_vec3_uniform(glGetUniformLocation(shader.program, "directional_light.ambient"), (vec3) {0.1f, 0.1f, 0.1f});
    shader_set_vec3_uniform(glGetUniformLocation(shader.program, "directional_light.diffuse"), (vec3) {0.5f, 0.5f, 0.5f});
    shader_set_vec3_uniform(glGetUniformLocation(shader.program, "directional_light.specular"), (vec3) {1.0f, 1.0f, 1.0f});
    shader_unbind();

    while (!window_should_close(window)) {
        window_update(window);

        if (window->resized) {
            camera_resize_projection(&camera, window_get_width(window), window_get_height(window));
            window->resized = false;
        }

        if (glfwGetKey(window->glfw_window, GLFW_KEY_W) == GLFW_PRESS) {
            camera_move_forward(&camera, window->delta_time);
        }
        if (glfwGetKey(window->glfw_window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_move_backward(&camera, window->delta_time);
        }
        if (glfwGetKey(window->glfw_window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_move_left(&camera, window->delta_time);
        }
        if (glfwGetKey(window->glfw_window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_move_right(&camera, window->delta_time);
        }

        if (window->mouse_captured) {
            double mouse_x, mouse_y;
            glfwGetCursorPos(window->glfw_window, &mouse_x, &mouse_y);
            camera_mouse_movement(&camera, mouse_x, mouse_y);
        }

        camera_update(&camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_bind(shader);
        shader_set_mat4_uniform(model_uniform_location, object_model);
        shader_set_mat4_uniform(view_uniform_location, camera.view);
        shader_set_mat4_uniform(projection_uniform_location, camera.projection);
        shader_set_vec3_uniform(view_position_uniform_location, camera.position);

        mesh_draw(&cube_mesh, shader);
    }

    shader_destroy(shader);
    window_destroy(window);

    return 0;
}

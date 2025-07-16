#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "model.h"
#include "window.h"
#include "shader.h"
#include "camera.h"

int main() {
    Window* window = window_create(1280, 720, "learn-opengl");

    Camera camera = camera_create((vec3) {0.0f, 0.0f, 0.0f}, window_get_width(window), window_get_height(window), 60.0f, 5.0f, 0.1f);

    Shader shader = shader_create("../shaders/depth.vert", "../shaders/depth.frag");

    Model model = model_create("../assets/models/sponza/sponza.obj", "../assets/models/sponza/");

    mat4 object_model = GLM_MAT4_IDENTITY_INIT;

    GLuint model_uniform_location = glGetUniformLocation(shader.program, "model");
    GLuint view_uniform_location = glGetUniformLocation(shader.program, "view");
    GLuint projection_uniform_location = glGetUniformLocation(shader.program, "projection");

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

        glm_mat4_identity(object_model);
        glm_scale(object_model, (vec3) {0.025f, 0.025f, 0.025f});
        shader_set_mat4_uniform(model_uniform_location, object_model);
        model_draw(&model, shader);
    }

    model_destroy(&model);
    shader_destroy(shader);
    window_destroy(window);

    return 0;
}

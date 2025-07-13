#include <stdbool.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "cglm/affine-pre.h"
#include "cglm/mat4.h"
#include "cglm/util.h"
#include "shader.h"
#include "texture.h"
#include "window.h"

int main() {
    Window* window = window_create(1280, 720, "learn-opengl");

    Shader shader = shader_create("../shaders/shader.vert", "../shaders/shader.frag");

    const GLfloat cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };


    vec3 cube_positions[] = {
        { 2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        { 2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        { 1.3f, -2.0f, -2.5f},
        { 1.5f,  2.0f, -2.5f},
        { 1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f}
    };

    GLuint vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    GLuint vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // unbinding the VAO first allows for the EBO to be automatically bound when the VAO gets bound
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Texture texture = texture_create("../assets/textures/wall.jpg");

    mat4 cube_model = GLM_MAT4_IDENTITY_INIT;

    mat4 view_matrix = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view_matrix, (vec3) {0.0f, 0.0f, -3.0f});
    mat4 projection_matrix;
    glm_perspective(glm_rad(60.0f), (float) window_get_width(window) / (float) window_get_height(window), 0.01f, 100.0f, projection_matrix);

    shader_bind(shader);
    GLuint model_uniform_location = glGetUniformLocation(shader.program, "model");
    GLuint view_uniform_location = glGetUniformLocation(shader.program, "view");
    GLuint projection_uniform_location = glGetUniformLocation(shader.program, "projection");

    shader_set_mat4_uniform(model_uniform_location, cube_model);
    shader_set_mat4_uniform(view_uniform_location, view_matrix);
    shader_set_mat4_uniform(projection_uniform_location, projection_matrix);
    shader_unbind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!window_should_close(window)) {
        window_update(window);

        if (window->resized) {
            glm_perspective(glm_rad(60.0f), (float) window_get_width(window) / (float) window_get_height(window), 0.01f, 100.0f, projection_matrix);
            shader_set_mat4_uniform(projection_uniform_location, projection_matrix);
            window->resized = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_bind(shader);
        glBindVertexArray(vertex_array_object);
        texture_bind(texture);

        glm_mat4_identity(cube_model);
        glm_rotate(cube_model, (float) glfwGetTime() * glm_rad(50.0f), (vec3) {0.5f, 1.0f, 0.0f});
        shader_set_mat4_uniform(model_uniform_location, cube_model);

        glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_vertices) / sizeof(GLfloat));
        for (int i = 0; i < sizeof(cube_positions) / sizeof(vec3); i++) {
            glm_mat4_identity(cube_model);
            glm_translate(cube_model, cube_positions[i]);
            shader_set_mat4_uniform(model_uniform_location, cube_model);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_vertices) / sizeof(GLfloat));
        }
    }

    texture_destroy(texture);
    shader_destroy(shader);
    window_destroy(window);

    return 0;
}

#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "cglm/affine-pre.h"
#include "cglm/mat4.h"
#include "cglm/util.h"
#include "window.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"

GLfloat cube_vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

vec3 cube_positions[] = {
    {  0.0f,  0.0f,  0.0f },
    {  2.0f,  5.0f, -15.0f },
    { -1.5f, -2.2f, -2.5f },
    { -3.8f, -2.0f, -12.3f },
    {  2.4f, -0.4f, -3.5f },
    { -1.7f,  3.0f, -7.5f },
    {  1.3f, -2.0f, -2.5f },
    {  1.5f,  2.0f, -2.5f },
    {  1.5f,  0.2f, -1.5f },
    { -1.3f,  1.0f, -1.5f }
};

int main() {
    Window* window = window_create(1280, 720, "learn-opengl");

    Camera camera = camera_create((vec3) {0.0f, 0.0f, 3.0f}, window_get_width(window), window_get_height(window), 60.0f, 5.0f, 0.1f);

    Shader object_shader = shader_create("../shaders/lighting.vert", "../shaders/lighting.frag");
    Shader light_shader = shader_create("../shaders/light.vert", "../shaders/light.frag");

    GLuint object_vao;
    glGenVertexArrays(1, &object_vao);
    glBindVertexArray(object_vao);

    GLuint vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    GLuint light_vao;
    glGenVertexArrays(1, &light_vao);
    glBindVertexArray(light_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Texture diffuse_texture = texture_create("../assets/textures/container.png");
    Texture specular_texture = texture_create("../assets/textures/container_specular.png");

    texture_bind(GL_TEXTURE0, diffuse_texture);
    texture_bind(GL_TEXTURE1, specular_texture);

    GLuint object_model_uniform_location = glGetUniformLocation(object_shader.program, "model");
    GLuint object_view_uniform_location = glGetUniformLocation(object_shader.program, "view");
    GLuint object_projection_uniform_location = glGetUniformLocation(object_shader.program, "projection");

    GLuint object_material_diffuse_uniform_location = glGetUniformLocation(object_shader.program, "material.diffuse");
    GLuint object_material_specular_uniform_location = glGetUniformLocation(object_shader.program, "material.specular");
    GLuint object_material_shininess_uniform_location = glGetUniformLocation(object_shader.program, "material.shininess");

    GLuint object_light_position_uniform_location = glGetUniformLocation(object_shader.program, "light.position");
    GLuint object_light_ambient_uniform_location = glGetUniformLocation(object_shader.program, "light.ambient");
    GLuint object_light_diffuse_uniform_location = glGetUniformLocation(object_shader.program, "light.diffuse");
    GLuint object_light_specular_uniform_location = glGetUniformLocation(object_shader.program, "light.specular");

    GLuint object_light_constant_uniform_location = glGetUniformLocation(object_shader.program, "light.constant");
    GLuint object_light_linear_uniform_location = glGetUniformLocation(object_shader.program, "light.linear");
    GLuint object_light_quadratic_uniform_location = glGetUniformLocation(object_shader.program, "light.quadratic");

    GLuint object_view_position_uniform_location = glGetUniformLocation(object_shader.program, "view_position");

    GLuint light_model_uniform_location = glGetUniformLocation(light_shader.program, "model");
    GLuint light_view_uniform_location = glGetUniformLocation(light_shader.program, "view");
    GLuint light_projection_uniform_location = glGetUniformLocation(light_shader.program, "projection");

    GLuint light_color_uniform_location = glGetUniformLocation(light_shader.program, "light_color");

    mat4 object_model = GLM_MAT4_IDENTITY_INIT;
    mat4 light_model = GLM_MAT4_IDENTITY_INIT;

    vec3 light_position = {1.2f, 1.0f, 2.0f};
    glm_translate(light_model, light_position);

    shader_bind(object_shader);
    shader_set_int_uniform(object_material_diffuse_uniform_location, 0);
    shader_set_int_uniform(object_material_specular_uniform_location, 1);
    shader_set_float_uniform(object_material_shininess_uniform_location, 32.0f);

    shader_set_vec3_uniform(object_light_position_uniform_location, light_position);
    shader_set_vec3_uniform(object_light_ambient_uniform_location, (vec3) {0.2f, 0.2f, 0.2f});
    shader_set_vec3_uniform(object_light_diffuse_uniform_location, (vec3) {0.5f, 0.5f, 0.5f});
    shader_set_vec3_uniform(object_light_specular_uniform_location, (vec3) {1.0f, 1.0f, 1.0f});
    shader_set_float_uniform(object_light_constant_uniform_location, 1.0f);
    shader_set_float_uniform(object_light_linear_uniform_location, 0.09f);
    shader_set_float_uniform(object_light_quadratic_uniform_location, 0.032f);

    shader_set_vec3_uniform(object_view_position_uniform_location, camera.position);

    shader_bind(light_shader);
    shader_set_vec3_uniform(light_color_uniform_location, (vec3) {1.0f, 1.0f, 1.0f});

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

        shader_bind(object_shader);
        shader_set_mat4_uniform(object_view_uniform_location, camera.view);
        shader_set_mat4_uniform(object_projection_uniform_location, camera.projection);
        shader_set_vec3_uniform(object_view_position_uniform_location, camera.position);

        glBindVertexArray(object_vao);
        for (int i = 0; i < sizeof(cube_positions) / sizeof(vec3); i++) {
            glm_mat4_identity(object_model);
            glm_translate(object_model, cube_positions[i]);
            float angle = 20.0f * i;
            glm_rotate(object_model, glm_rad(angle), (vec3) {1.0f, 0.3f, 0.5f});
            shader_set_mat4_uniform(object_model_uniform_location, object_model);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_vertices) / sizeof(GLfloat));
        }

        shader_bind(light_shader);
        shader_set_mat4_uniform(light_model_uniform_location, light_model);
        shader_set_mat4_uniform(light_view_uniform_location, camera.view);
        shader_set_mat4_uniform(light_projection_uniform_location, camera.projection);

        glBindVertexArray(light_vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_vertices) / sizeof(GLfloat));
    }

    shader_destroy(object_shader);
    shader_destroy(light_shader);
    window_destroy(window);

    return 0;
}

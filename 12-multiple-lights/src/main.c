#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "cglm/affine-pre.h"
#include "cglm/mat4.h"
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

vec3 light_positions[] = {
    {0.7f,  0.2f,  2.0f},
    {2.3f, -3.3f, -4.0f},
    {-4.0f,  2.0f, -12.0f},
    {0.0f,  0.0f, -3.0f}
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

    mat4 object_model = GLM_MAT4_IDENTITY_INIT;
    mat4 light_model = GLM_MAT4_IDENTITY_INIT;

    GLuint object_model_uniform_location = glGetUniformLocation(object_shader.program, "model");
    GLuint object_view_uniform_location = glGetUniformLocation(object_shader.program, "view");
    GLuint object_projection_uniform_location = glGetUniformLocation(object_shader.program, "projection");
    GLuint light_model_uniform_location = glGetUniformLocation(light_shader.program, "model");
    GLuint light_view_uniform_location = glGetUniformLocation(light_shader.program, "view");
    GLuint light_projection_uniform_location = glGetUniformLocation(light_shader.program, "projection");

    shader_bind(object_shader);
    shader_set_int_uniform(glGetUniformLocation(object_shader.program, "material.diffuse"), 0);
    shader_set_int_uniform(glGetUniformLocation(object_shader.program, "material.specular"), 1);
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "material.shininess"), 32.0f);

    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "directional_light.direction"), (vec3) {-0.2f, -1.0f, -0.3f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "directional_light.ambient"), (vec3) {0.1f, 0.1f, 0.1f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "directional_light.diffuse"), (vec3) {0.5f, 0.5f, 0.5f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "directional_light.specular"), (vec3) {1.0f, 1.0f, 1.0f});

    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[0].position"), (vec3) {0.7f,  0.2f,  2.0f});
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[0].constant"), 1.0f);
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[0].linear"), 0.09f);
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[0].quadratic"), 0.032f);
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[0].ambient"), (vec3) {0.1f, 0.1f, 0.1f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[0].diffuse"), (vec3) {0.5f, 0.5f, 0.5f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[0].specular"), (vec3) {1.0f, 1.0f, 1.0f});

    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[1].position"), (vec3) {2.3f, -3.3f, -4.0f});
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[1].constant"), 1.0f);
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[1].linear"), 0.09f);
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[1].quadratic"), 0.032f);
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[1].ambient"), (vec3) {0.1f, 0.1f, 0.1f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[1].diffuse"), (vec3) {0.5f, 0.5f, 0.5f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[1].specular"), (vec3) {1.0f, 1.0f, 1.0f});

    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[2].position"), (vec3) {-4.0f,  2.0f, -12.0f});
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[2].constant"), 1.0f);
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[2].linear"), 0.09f);
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[2].quadratic"), 0.032f);
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[2].ambient"), (vec3) {0.1f, 0.1f, 0.1f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[2].diffuse"), (vec3) {0.5f, 0.5f, 0.5f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[2].specular"), (vec3) {1.0f, 1.0f, 1.0f});

    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[3].position"), (vec3) {0.0f,  0.0f, -3.0f});
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[3].constant"), 1.0f);
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[3].linear"), 0.09f);
    shader_set_float_uniform(glGetUniformLocation(object_shader.program, "point_lights[3].quadratic"), 0.032f);
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[3].ambient"), (vec3) {0.1f, 0.1f, 0.1f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[3].diffuse"), (vec3) {0.5f, 0.5f, 0.5f});
    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "point_lights[3].specular"), (vec3) {1.0f, 1.0f, 1.0f});

    shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "view_position"), camera.position);

    shader_bind(light_shader);
    shader_set_vec3_uniform(glGetUniformLocation(light_shader.program, "light_color"), (vec3) {1.0f, 1.0f, 1.0f});

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
        shader_set_vec3_uniform(glGetUniformLocation(object_shader.program, "view_position"), camera.position);

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
        shader_set_mat4_uniform(light_view_uniform_location, camera.view);
        shader_set_mat4_uniform(light_projection_uniform_location, camera.projection);

        glBindVertexArray(light_vao);

        for (int i = 0 ; i < 4; i++) {
            glm_mat4_identity(light_model);
            glm_translate(light_model, light_positions[i]);

            shader_set_mat4_uniform(light_model_uniform_location, light_model);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_vertices) / sizeof(GLfloat));
        }
    }

    shader_destroy(object_shader);
    shader_destroy(light_shader);
    window_destroy(window);

    return 0;
}

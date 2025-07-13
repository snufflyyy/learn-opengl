#include "shader.h"
#include "texture.h"
#include "window.h"

#include <stdio.h>

#include <glad/glad.h>

int main() {
    Window* window = window_create(1280, 720, "learn-opengl");

    Shader shader = shader_create("../shaders/shader.vert", "../shaders/shader.frag");

    const GLfloat square_vertices[] = {
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f // top left
    };
    const GLuint square_indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    GLuint vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    GLuint vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);

    GLuint element_buffer_object;
    glGenBuffers(1, &element_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_indices), square_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // unbinding the VAO first allows for the EBO to be automatically bound when the VAO gets bound
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Texture texture = texture_create("../assets/textures/wall.jpg");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!window_should_close(window)) {
        window_update(window);

        glClear(GL_COLOR_BUFFER_BIT);
        shader_bind(shader);
        glBindVertexArray(vertex_array_object);
        texture_bind(texture);
        glDrawElements(GL_TRIANGLES, sizeof(square_indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    }

    window_destroy(window);

    return 0;
}

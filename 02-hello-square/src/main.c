#include "window.h"

#include <stdio.h>

#include <glad/glad.h>

int main() {
    Window* window = window_create(1280, 720, "learn-opengl");

    const char* vertex_shader_source = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main() {\n"
        "   gl_Position = vec4(position, 1.0);\n"
        "}\0";

    const char* fragment_shader_source = "#version 330 core\n"
        "out vec4 fragment_color;\n"
        "void main() {\n"
        "   fragment_color = vec4(0.0, 0.5, 1.0, 1.0);\n"
        "}\0";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);

    glCompileShader(vertex_shader);
    GLint vertex_shader_compile_status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_compile_status);
    if (vertex_shader_compile_status == GL_FALSE) {
        char info_log[512];
        glGetShaderInfoLog(vertex_shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "[ERROR] [Shader] Failed to compile vertex shader: \n%s\n", info_log);
    } else {
        printf("[INFO] [Shader] Compiled vertex shader!\n");
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);

    glCompileShader(fragment_shader);
    GLint fragment_shader_compile_status;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_shader_compile_status);
    if (fragment_shader_compile_status == GL_FALSE) {
        char info_log[512];
        glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "[ERROR] [Shader] Failed to compile fragment shader: \n%s\n", info_log);
    } else {
        printf("[INFO] [Shader] Compiled fragment shader!\n");
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    GLint shader_program_link_status;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &shader_program_link_status);
    if (shader_program_link_status == GL_FALSE) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "[ERROR] [Shader] Failed to link shader program: \n%s\n", info_log);
    } else {
        printf("[INFO] [Shader] Linked shader program!\n");
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    const GLfloat square_vertices[] = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f  // top left
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(0);

    // unbinding the VAO first allows for the EBO to be automatically bound when the VAO gets bound
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!window_should_close(window)) {
        window_update(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);
        glBindVertexArray(vertex_array_object);
        glDrawElements(GL_TRIANGLES, sizeof(square_indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    }

    window_destroy(window);

    return 0;
}

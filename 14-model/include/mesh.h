#pragma once

#include <cglm/cglm.h>
#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

typedef struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texture_coord;
} Vertex;

typedef struct Mesh {
    Vertex* vertices;
    unsigned int number_of_vertices;

    GLuint* indices;
    unsigned int number_of_indices;

    Texture diffuse_texture;
    Texture specular_texture;

    GLuint VAO, VBO, EBO;
} Mesh;

Mesh mesh_create(Vertex* vertices, unsigned int number_of_vertices, GLuint* indices, unsigned int number_of_indices, Texture diffuse_texture, Texture specular_texture);
void mesh_draw(Mesh* mesh, Shader shader);
void mesh_destroy(Mesh* mesh);

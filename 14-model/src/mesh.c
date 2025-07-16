#include "mesh.h"
#include "shader.h"
#include "texture.h"

#include <glad/glad.h>

Mesh mesh_create(Vertex* vertices, unsigned int number_of_vertices, GLuint* indices, unsigned int number_of_indices, Texture diffuse_texture, Texture specular_texture) {
    Mesh mesh = {0};

    mesh.vertices = vertices;
    mesh.number_of_vertices = number_of_vertices;

    mesh.indices = indices;
    mesh.number_of_indices = number_of_indices;

    mesh.diffuse_texture = diffuse_texture;
    mesh.specular_texture = specular_texture;

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

    glBufferData(GL_ARRAY_BUFFER, mesh.number_of_vertices * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.number_of_indices * sizeof(GLuint), &mesh.indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texture_coord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    printf("[INFO] [Mesh] Created mesh!\n");
    return mesh;
}

void mesh_draw(Mesh* mesh, Shader shader) {
    texture_bind(GL_TEXTURE0, mesh->diffuse_texture);
    shader_set_int_uniform(glGetUniformLocation(shader.program, "material.diffuse"), 0);
    texture_bind(GL_TEXTURE1, mesh->specular_texture);
    shader_set_int_uniform(glGetUniformLocation(shader.program, "material.specular"), 1);

    shader_bind(shader);
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->number_of_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void mesh_destroy(Mesh* mesh) {
    glDeleteBuffers(1, &mesh->EBO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteVertexArrays(1, &mesh->VAO);

    texture_destroy(mesh->diffuse_texture);
    texture_destroy(mesh->specular_texture);
}

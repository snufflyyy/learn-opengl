#pragma once

#include <glad/glad.h>

typedef struct Texture {
    GLuint id;
} Texture;

Texture texture_create(const char* image_path);
void texture_bind(Texture texture);
void texture_unbind();
void texture_destroy(Texture texture);

#pragma once

#include "mesh.h"
#include "shader.h"

#define MAX_TEXTURE_PATH 512

typedef struct LoadedTexture {
    Texture texture;
    char texture_path[MAX_TEXTURE_PATH];
} LoadedTexture;

typedef struct Model {
    Mesh* meshes;
    unsigned int number_of_meshes;

    LoadedTexture* loaded_textures;
    unsigned int number_of_loaded_textures;
} Model;

Model model_create(const char* model_path, const char* model_root_path);
void model_draw(Model* model, Shader shader);
void model_destroy(Model* model);

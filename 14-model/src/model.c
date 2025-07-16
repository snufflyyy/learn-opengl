#include "model.h"

#include <stdio.h>
#include <string.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assimp/material.h"
#include "mesh.h"
#include "texture.h"

static void model_load(Model* model, const char* model_path, const char* model_root_path);
static void model_process_node(Model* model, struct aiNode* node, const struct aiScene* scene, int* mesh_index, const char* model_root_path);
static Mesh model_process_mesh(Model* model, struct aiMesh* mesh, const struct aiScene* scene, const char* model_root_path);
static Texture model_load_material_texture(Model* model, struct aiMaterial* material, enum aiTextureType type, const char* model_root_path);

Model model_create(const char* model_path, const char* model_root_path) {
    Model model = {0};
    model_load(&model, model_path, model_root_path);

    printf("[INFO] [Model] Loaded model!\n");
    return model;
}

static void model_load(Model* model, const char* model_path, const char* model_root_path) {
    const struct aiScene* scene = aiImportFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fprintf(stderr, "[ERROR] [Model] Failed to import model: \n%s\n", aiGetErrorString());
        return;
    }

    model->number_of_meshes = scene->mNumMeshes;
    model->meshes = (Mesh*) malloc(sizeof(Mesh) * model->number_of_meshes);
    if (!model->meshes) {
        fprintf(stderr, "[ERROR] [Model] Failed to allocate memory for meshes!\n");
        return;
    }

    int mesh_index = 0;
    model_process_node(model, scene->mRootNode, scene, &mesh_index, model_root_path);

    aiReleaseImport(scene);
}

static void model_process_node(Model* model, struct aiNode* node, const struct aiScene* scene, int* mesh_index, const char* model_root_path) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes[*mesh_index] = model_process_mesh(model, mesh, scene, model_root_path);
        (*mesh_index)++;
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        model_process_node(model, node->mChildren[i], scene, mesh_index, model_root_path);
    }
}

static Mesh model_process_mesh(Model* model, struct aiMesh* mesh, const struct aiScene* scene, const char* model_root_path) {
    Vertex* vertices = (Vertex*) malloc(sizeof(Vertex) * mesh->mNumVertices);
    if (!vertices) {
        fprintf(stderr, "[ERROR] [Model] Failed to allocate memory for mesh's vertices!\n");
        return (Mesh) {0};
    }
    unsigned int number_of_vertices = mesh->mNumVertices;

    // process vertices
    for (unsigned int i = 0; i < number_of_vertices; i++) {
        vertices[i] = (Vertex) {{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z}, {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z}, {0.0f, 0.0f}};
        if (mesh->mTextureCoords[0]) {
            vertices[i].texture_coord[0] = mesh->mTextureCoords[0][i].x;
            vertices[i].texture_coord[1] = mesh->mTextureCoords[0][i].y;
        }
    }

    unsigned int number_of_indices = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        number_of_indices += mesh->mFaces[i].mNumIndices;
    }

    GLuint* indices = (GLuint*) malloc(sizeof(GLuint) * number_of_indices);
    if (!indices) {
        fprintf(stderr, "[ERROR] [Model] Failed to allocate memory for mesh's indices!\n");
        free(vertices);
        return (Mesh) {0};
    }

    // process indices
    unsigned int indices_index = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            indices[indices_index++] = mesh->mFaces[i].mIndices[j];
        }
    }

    Texture diffuse_texture = {0};
    Texture specular_texture = {0};

    if (mesh->mMaterialIndex >= 0) {
        struct aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        diffuse_texture = model_load_material_texture(model, material, aiTextureType_DIFFUSE, model_root_path);
        specular_texture = model_load_material_texture(model, material, aiTextureType_SPECULAR, model_root_path);
    }

    return mesh_create(vertices, number_of_vertices, indices, number_of_indices, diffuse_texture, specular_texture);
}

static Texture model_load_material_texture(Model* model, struct aiMaterial* material, enum aiTextureType type, const char* model_root_path) {
    struct aiString string;
    aiGetMaterialTexture(material, type, 0, &string, NULL, NULL, NULL, NULL, NULL, NULL);

    char texture_path[MAX_TEXTURE_PATH];
    snprintf(texture_path, MAX_TEXTURE_PATH, "%s%s", model_root_path, string.data);

    for (int i = 0; i < model->number_of_loaded_textures; i++) {
        if (strncmp(texture_path, model->loaded_textures[i].texture_path, MAX_TEXTURE_PATH) == 0) {
            return model->loaded_textures[i].texture;
        }
    }

    Texture texture = texture_create(texture_path);

    void* temp = realloc(model->loaded_textures, sizeof(LoadedTexture) * (model->number_of_loaded_textures + 1));
    if (!temp) {
        fprintf(stderr, "[ERROR] [Model] Failed to reallocate memory for all loaded model textures!\n");
        return (Texture) {0};
    }

    model->loaded_textures = (LoadedTexture*) temp;

    strncpy(model->loaded_textures[model->number_of_loaded_textures].texture_path, texture_path, MAX_TEXTURE_PATH);
    model->loaded_textures[model->number_of_loaded_textures].texture = texture;
    model->number_of_loaded_textures++;

    return texture;
}

void model_draw(Model* model, Shader shader) {
    for (int i = 0; i < model->number_of_meshes; i++) {
        mesh_draw(&model->meshes[i], shader);
    }
}

void model_destroy(Model* model) {
    for (int i = 0; i < model->number_of_meshes; i++) {
        mesh_destroy(&model->meshes[i]);

        free(model->meshes[i].vertices);
        model->meshes[i].vertices = NULL;
        model->meshes[i].number_of_vertices = 0;

        free(model->meshes[i].indices);
        model->meshes[i].indices = NULL;
        model->meshes[i].number_of_indices = 0;
    }

    free(model->meshes);
    model->meshes = NULL;
    model->number_of_meshes = 0;

    free(model->loaded_textures);
    model->loaded_textures = NULL;
    model->number_of_loaded_textures = 0;

    printf("[INFO] [Model] Model destroyed!\n");
}

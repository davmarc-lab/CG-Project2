#pragma once

#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>


enum Flip { VERTICALLY, KEEP };

class Object {
private:
    vector<InfoTexture> textures_loaded;
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    void loadModel(string path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    vector<InfoTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                             string typeName);

public:
    Object(const char *path, Flip flip = Flip::KEEP, bool gamma = false);

    Object() {}

    void draw(Shader shader);
};

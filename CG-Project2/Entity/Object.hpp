#pragma once

#include "Entity.hpp"
#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>

enum Flip { VERTICALLY, KEEP };

/*
 * This class can be used to import an obj file into the Scene, using the material defined by the mtl file.
 * */
class Object : public Entity {
  private:
    vector<InfoTexture> textures_loaded;
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    void loadModel(string path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    vector<InfoTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

  public:
    // Constructs an object using or a path to the obj file, or a path and a combination of other parameters.
    Object(const char *path, Flip flip = Flip::KEEP, bool gamma = false);

    Object() {}

    virtual void createVertexArray() override {}

    virtual void draw(Shader shader) override;
};

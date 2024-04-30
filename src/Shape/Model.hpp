#pragma once

#include "../Lib.hpp"
#include "../include/texture/stb_image.hpp"
#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <vector>

enum Flip {
    VERTICALLY,
    KEEP
};

class Model
{
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
        Model(const char* path, Flip flip = Flip::KEEP, bool gamma = false) : gammaCorrection(gamma)
        {
            if (flip == Flip::VERTICALLY)
            {
                stbi_set_flip_vertically_on_load(true);
            }
            this->loadModel(path);
        }
        
        Model() {}

        void draw(Shader shader);
};

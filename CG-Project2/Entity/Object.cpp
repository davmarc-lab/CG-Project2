#include "Object.hpp"
#include "Mesh.hpp"

// texture header file
#ifdef _WIN32
#include <texture/stb_image.hpp>
#else
#include "../dependencies/include/texture/stb_image.hpp"
#endif

Object::Object(const char *path, Flip flip, bool gamma) : gammaCorrection(gamma) {
    if (flip == Flip::VERTICALLY) {
        stbi_set_flip_vertically_on_load(true);
    }
    this->loadModel(path);

    this->m_isInstaced = true;
}

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

void Object::draw(Shader shader) {
    shader.use();
    shader.setMat4("model", this->getModelMatrix());
    // Object material
    shader.setVec3("material.ambient", this->material.getAmbient());
    shader.setVec3("material.diffuse", this->material.getDiffuse());
    shader.setVec3("material.specular", this->material.getSpecular());
    shader.setFloat("material.shininess", this->material.getShininess());

    for (int i = 0; i < this->meshes.size(); i++) {
        this->meshes[i].draw(shader);
    }
}

void Object::loadModel(string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Object::processNode(aiNode *node, const aiScene *scene) {
    // it process all the nodes of the meshes
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(processMesh(mesh, scene));
    }

    // then it does the same for each of its children
    for (int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Object::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<InfoVertex> vertices;
    vector<unsigned int> indices;
    vector<InfoTexture> textures;

    for (int i = 0; i < mesh->mNumVertices; i++) {
        InfoVertex vertex;
        // vertex position
        vec3 vec = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.position = vec;

        // normal vectors
        if (mesh->HasNormals()) {
            vec = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            vertex.normal = vec;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            vec2 tex = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertex.texCoords = tex;

            // tangent
            vec = vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            vertex.tangent = vec;

            // biTangent
            vec = vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            vertex.biTangent = vec;
        } else {
            vertex.texCoords = vec2(0);
        }

        vertices.push_back(vertex);
    }

    // process the index for the ebo buffer
    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process materials
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    vector<InfoTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    vector<InfoTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    vector<InfoTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    vector<InfoTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
}

vector<InfoTexture> Object::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
    vector<InfoTexture> textures;

    for (int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            InfoTexture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma) {
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        cout << "Texture failed to load at path: " << path << endl;
        stbi_image_free(data);
    }

    return textureID;
}

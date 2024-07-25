#pragma once

#include "../Lib.hpp"
#include "../Shader/Shader.hpp"

#include <vector>

#define MAX_BONE_INFLUENCE 4

struct InfoVertex {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
    vec3 tangent;
    vec3 biTangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct InfoTexture {
    GLuint id;
    string type;
    string path;
};

class Mesh {
  private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    vector<InfoVertex> vertex;
    vector<unsigned int> index;
    vector<InfoTexture> texture;

    unsigned int m_ndiffuse = 1;
    unsigned int m_nspecular = 1;
    unsigned int m_nnormal = 1;
    unsigned int m_nheight = 1;

    string m_number;
    string m_name;

    void createVertexArray();

  public:
    Mesh() {}

    Mesh(vector<InfoVertex> vertex, vector<unsigned int> index, vector<InfoTexture> texture);

    void draw(Shader shader);
};

#include "Mesh.hpp"
#include <cstddef>

Mesh::Mesh(vector<InfoVertex> vertex, vector<unsigned int> index, vector<InfoTexture> texture)
{
    this->vertex = vertex;
    this->index = index;
    this->texture = texture;

    this->createVertexArray();
}

void Mesh::createVertexArray()
{
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertex.size() * sizeof(InfoVertex), &this->vertex[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index.size() * sizeof(unsigned int), &this->index[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void*)offsetof(InfoVertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void*)offsetof(InfoVertex, texCoords));
    glEnableVertexAttribArray(2);

    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void*)offsetof(InfoVertex, tangent));

    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void*)offsetof(InfoVertex, biTangent));

    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(InfoVertex), (void*)offsetof(InfoVertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void*)offsetof(InfoVertex, m_Weights));

    glBindVertexArray(0);
}

void Mesh::draw(Shader shader)
{
    unsigned int ndiffuse = 1;
    unsigned int nspecular = 1;
    unsigned int nnormal = 1;
    unsigned int nheight = 1;

    for (int i = 0; i < this->texture.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);       // activate the proper texture unit

        string number;
        string name = this->texture[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(ndiffuse++);
        else if(name == "texture_specular")
            number = std::to_string(nspecular++); // transfer unsigned int to string
        else if(name == "texture_normal")
            number = std::to_string(nnormal++); // transfer unsigned int to string
        else if(name == "texture_height")
            number = std::to_string(nheight++); // transfer unsigned int to string

        shader.setInt((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, this->texture[i].id);
    }

    // draw mesh
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->index.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

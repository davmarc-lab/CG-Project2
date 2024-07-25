#include "Mesh.hpp"
#include <cstddef>

Mesh::Mesh(vector<InfoVertex> vertex, vector<unsigned int> index, vector<InfoTexture> texture) {
    this->vertex = vertex;
    this->index = index;
    this->texture = texture;

    this->createVertexArray();
}

void Mesh::createVertexArray() {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertex.size() * sizeof(InfoVertex), &this->vertex[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index.size() * sizeof(unsigned int), &this->index[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void *)offsetof(InfoVertex, texCoords));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void *)offsetof(InfoVertex, normal));
    glEnableVertexAttribArray(3);

    // vertex tangent
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void *)offsetof(InfoVertex, tangent));

    // vertex bitangent
    // glEnableVertexAttribArray(4);
    // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void *)offsetof(InfoVertex, biTangent));

    // ids
    // glEnableVertexAttribArray(5);
    // glVertexAttribIPointer(5, 4, GL_INT, sizeof(InfoVertex), (void *)offsetof(InfoVertex, m_BoneIDs));

    // weights
    // glEnableVertexAttribArray(6);
    // glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InfoVertex), (void *)offsetof(InfoVertex, m_Weights));

    glBindVertexArray(0);
}

void Mesh::draw(Shader shader) {
    shader.use();

    for (int i = 0; i < this->texture.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // activate the proper texture unit

        this->m_name = this->texture[i].type;
        if (this->m_name == "texture_diffuse")
            m_number = std::to_string(this->m_ndiffuse++);
        else if (this->m_name == "texture_specular")
            m_number = std::to_string(this->m_nspecular++); // transfer unsigned int to string
        else if (this->m_name == "texture_normal")
            m_number = std::to_string(this->m_nnormal++); // transfer unsigned int to string
        else if (this->m_name == "texture_height")
            m_number = std::to_string(this->m_nheight++); // transfer unsigned int to string

        shader.setInt((this->m_name + m_number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, this->texture[i].id);
    }

    // draw mesh
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->index.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

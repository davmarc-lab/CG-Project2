#include "Cube.hpp"

inline float vertices[] = { // coords.vertex coord,   texture coord,  coords.normals vectors
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  0.0f,  -1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f
};

void CubeEntity::buildCube() {
    int size = sizeof(vertices) / sizeof(float);
    for (int i = 0; i < size; i += 8) {
        this->coords.vertex.push_back(vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
        this->coords.texCoords.push_back(vec2(vertices[i + 3], vertices[i + 4]));
        this->coords.normals.push_back(vec3(vertices[i + 5], vertices[i + 6], vertices[i + 7]));
        this->coords.colors.push_back(this->color.getColorVector());
    }
}

void CubeEntity::createVertexArray() {
    glGenVertexArrays(1, &this->buffers.vao);

    // vbo geometry
    glGenBuffers(1, &this->buffers.vbo_g);
    glBindVertexArray(this->buffers.vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo_g);
    glBufferData(GL_ARRAY_BUFFER, this->coords.vertex.size() * sizeof(vec3), this->coords.vertex.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    // vbo coords.colors
    glGenBuffers(1, &this->buffers.vbo_c);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo_c);
    glBufferData(GL_ARRAY_BUFFER, this->coords.colors.size() * sizeof(vec4), this->coords.colors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    // vbo normals
    glGenBuffers(1, &this->buffers.vbo_n);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo_n);
    glBufferData(GL_ARRAY_BUFFER, this->coords.normals.size() * sizeof(vec3), this->coords.normals.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(3);

    this->m_isInstaced = true;
}

void CubeEntity::draw(Shader shader) {
    shader.use();

    // Entity model and projection
    shader.setMat4("projection", projection);
    shader.setMat4("model", this->getModelMatrix());



    // Entity material
    shader.setVec3("material.ambient", this->material.getAmbient());
    shader.setVec3("material.diffuse", this->material.getDiffuse());
    shader.setVec3("material.specular", this->material.getSpecular());
    shader.setFloat("material.shininess", this->material.getShininess());

    // Lights
    if (string(this->texture.getPath()) != "None") {
        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture.getId());
    }

    glBindVertexArray(this->buffers.vao);
    glDrawArrays(GL_TRIANGLES, 0, this->coords.vertex.size());
    glBindVertexArray(0);
}

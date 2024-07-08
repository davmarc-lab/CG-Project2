#include "Sphere.hpp"


void Sphere::buildSphere() {
    vec3 center = vec3(0.0, 0.0, 0.0);
	vec3 radius = vec3(1.0, 1.0, 1.0);
	float s, t;

	for (int i = 0; i <= this->stacks; ++i) {

		float V = i / (float)this->stacks;
		float phi = V * 3.14;

		for (int j = 0; j <= this->slices; ++j) {

			float U = j / (float)this->slices;
			float theta = U * (pi <float>() * 2);

			float x = center.x + radius.x * (cosf(theta) * sinf(phi));
			float y = center.y + radius.y * cosf(phi);
			float z = center.z + radius.z * sinf(theta) * sinf(phi);

			this->coords.vertex.push_back(vec3(x, y, z));
			this->coords.colors.push_back(this->color.getColorVector());
			this->coords.normals.push_back(vec3(x, y, z));

			s = U;
			t = V;
			this->coords.texCoords.push_back(vec2(s, t));
		}
	}

	for (int i = 0; i < this->slices * this->stacks + this->slices; ++i) {

		this->index.push_back(i);
		this->index.push_back(i + this->slices + 1);
		this->index.push_back(i + this->slices);

		this->index.push_back(i + this->slices + 1);
		this->index.push_back(i);
		this->index.push_back(i + 1);
	}

	this->coords.vertex.push_back(vec3(0.0, 0.0, 0.0));
	this->coords.colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	int nv = this->coords.vertex.size();
	this->index.push_back(nv - 1);
}

void Sphere::createVertexArray() {
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

    glGenBuffers(1, &this->buffers.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index.size() * sizeof(GLuint), this->index.data(), GL_STATIC_DRAW);
}

void Sphere::draw(Shader shader) {
    shader.use();
    shader.setMat4("model", this->transform.getModelMatrix());

    glBindVertexArray(this->buffers.vao);
    glDrawElements(GL_TRIANGLES, (this->index.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

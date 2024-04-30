#include "Chunk.hpp"

Chunk::Chunk(const int id)
{
    this->id = id;
}

void Chunk::generateChunk()
{
    Texture texture = Texture("resources/textures/web-dirt.png", Cube().getTextureCoords());
    texture.createTexture();

    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        for (int j = 0; j < NUM_BLOCKS; j++)
        {
            Shape3D* cube = new Cube(color::RED);
            cube->createVertexArray();
            // position => vec3(startChunk + row * xscale * 2, 0, startChunk + col * yscale * 2)
            cube->transformMesh(vec3(0 + i * 0.1 * 2, 0, 0 + j * 0.1 * 2), vec3(0.1), vec3(0), 0);
            cube->attachTexture(texture);

            this->elements.push_back(cube);
        }
    }
}

void Chunk::drawChunk(Shader shader)
{
    for (const auto shape: this->elements)
    {
        shape->draw(shader);
    }
}

void Chunk::updateChunk()
{

}

#pragma once

#include "../Shape/Shape3D.hpp"
#include "../Shape/Cube.hpp"

#define NUM_BLOCKS 6

class Chunk {
private:
    unsigned int id;
    vector<Shape3D*> elements;

public:
    Chunk(const int id);

    void generateChunk();

    void updateChunk();

    unsigned int getId() { return this->id; }

    int getNumShapes() { return this->elements.size(); }

    inline vector<Shape3D*> getShapes() { return this->elements; }

    void drawChunk(Shader shader);
};

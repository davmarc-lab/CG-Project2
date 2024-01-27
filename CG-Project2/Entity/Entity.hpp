#pragma once

#include "../Lib.hpp"

#include "Object.hpp"
#include "../Buffers/VAO.hpp"
#include "../Buffers/VBO.hpp"
#include "../Component/TransformComponent.hpp"

#include <unordered_map>

struct Buffers
{
    VAO vao;
    VBO vbo;
};

struct Vertex
{
    vector<vec3> position;
    vector<vec4> colors;
    vector<vec2> texCoord;
    vector<vec3> normals;
};

class Entity
{
    private:
        unsigned int currentId = 0;

        Buffers buffers;

        Vertex vertices;

        vector<Object> entities;

        // Components
        unordered_map<unsigned int, TransformComponent> transformComponent;

    public:
        ~Entity() {}

        Entity() {}

        /*Retrieves the current id of the object added.*/
        int createEntity(Object *obj)
        {
            obj->setId(this->currentId);
            this->entities.push_back(*obj);
            return this->currentId++;
        }

        Vertex getVertexInfo() { return this->vertices; }


        void setVerticesPosition(const vector<vec3> pos) { this->vertices.position = pos; }
        
        void setVerticesColor(const vector<vec4> color) { this->vertices.colors = color; }

        void setVerticesTextureCoord(const vector<vec2> texCoord) { this->vertices.texCoord = texCoord; }
        
        void setVerticesNormals(const vector<vec3> normal) { this->vertices.normals = normal; }

};

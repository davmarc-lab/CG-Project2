#pragma once

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "../Shader/Shader.hpp"
#include "Model/Transform.hpp"
#include "Textures/Texture.hpp"
#include <vector>

/*
 * This abstract class provides a standard base for the most of basic shapes using VAO, geometry and
 * color VBO, mid and other color, modellation matrix.
 */
class Shape3D
{
    public:
        /* ---Fields--- */

        // Standard VAO buffer.
        GLuint vao;
        // VBO buffer used for geometry.
        GLuint vbo_g;
        // VBO buffer used for colors.
        GLuint vbo_c;
        // VBO buffer used for texCoords.
        GLuint vbo_t;
        // VBO buffer used for normals.
        GLuint vbo_n;        
        // Vector of all vertices of the shape.
        vector<vec3> vertex;
        // Vector of all color for each vertex.
        vector<vec4> colors;
        // Vector of TexCoords.
        vector<vec2> texCoord;
        // Vector of normal vectors for each vertex.
        vector<vec3> normal;
        // The primary color of the shape.
        Color color;
        // The center vertex color of the shape.
        Color midColor;
        // Transform the model matrix.
        Transform transform;
        // Texture object.
        Texture texture;
        // Number of vertex of the shape.
        int nvertex = 0;
        // Number of triangles of the shape.
        int ntriangle = 0;
        // Says if the shape is solid or not
        bool isSolid = false;
        // Says if the shape is destroyed or alive
        bool isDestroyed = false;

        /* ---Mehtods--- */

        // Add element to vertex vector3.
        void addElementVertex(vec3 elem) { this->vertex.push_back(elem); }

        // Add element to colors vetcor4.
        void addElementColors(vec4 elem) { this->colors.push_back(elem); }

        // Retrieves the vertex number of the shape.
        int getVertexNum() { return this->nvertex; }

        // This method set the number of vertices.
        void setVertexNum(int n) { this->nvertex = n; }

        // Retrieves the triangles nummber of the shape.
        int getTriangleNum() { return this->ntriangle; }

        // This method sets the number of triangles of the shape.
        void setTriangleNum(int n) { this->ntriangle = n; }
    
        void setTextureCoords(vector<vec2> coords) { this->texCoord = coords; }

        vector<vec2> getTextureCoords() { return this->texCoord; }

        void setNormalVector(vector<vec3> normal) { this->normal = normal; }
        
        vector<vec3> getNormalVector() { return this->normal; }

        // This method sets the primary color of the shape.
        void setColor(Color color) { this->color = color; }

        // Retrieves the primary color of the shape.
        Color getColor() { return this->color; }

        // This method sets the center vertex color of the shape.
        void setMidColor(Color mid) { this->midColor = mid; }

        // Retrieves the center vertex color of the shape.
        Color getMidColor() { return this->midColor; }

        // Retrieves the vector of vertices of the shape.
        vector<vec3> getVertexArray() { return this->vertex; }

        // This method sets the vector of vertices of the shape.
        void setVertexArray(vector<vec3> vertex) { this->vertex = vertex; }

        // This method sets the vector of colors of the shape.
        void setColorsArray(vector<vec4> colors) { this->colors = colors; }

        // Retrieves the modellation matrix of the shape.
        mat4 getModelMatrix() { return this->transform.getModelMatrix(); }

        // This metod sets the model matrix.
        void setModelMatrix(mat4 model) { this->transform.setModelMatrix(model); }

        void transformMesh(vec3 translateVector,
                vec3 scaleVector,
                vec3 rotateAxis,
                float rotationValue) 
        {
            this->transform.applyTransofrmation(translateVector,
                scaleVector,
                rotateAxis,
                rotationValue);
        }

        void attachTexture(Texture texture) {
            this->texture = texture;

            glBindVertexArray(this->vao);

            glGenBuffers(1, &this->vbo_t);
            glBindBuffer(GL_ARRAY_BUFFER, this->vbo_t);
            glBufferData(GL_ARRAY_BUFFER, this->texCoord.size() * sizeof(vec2), this->texCoord.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glEnableVertexAttribArray(2);

            glBindVertexArray(0);
        }

        void setSolid() { this->isSolid = true; }

        bool checkCollision(Shape3D* shape)
        {
            return false;
        }

        void setDestroyed() { this->isDestroyed = true; }

        void setAlive() { this->isDestroyed = false; }

        bool isAlive() { return !this->isDestroyed; }

        // Retrieves the VertexArrayObject of the shape.
        GLuint getVertexArrayObject() { return this->vao; }

        // Retrieves the vector of colors for each vertex.
        vector<vec4> getColorsArray() { return this->colors; }

        // This method clear the vertex vector.
        void clearVertexArray() { this->vertex.clear(); }

        // This method clear the color vector.
        void clearColorArray() { this->colors.clear(); }

        virtual void clearShape() = 0;

        // Creates the VAO of the shape
        virtual void createVertexArray() = 0;

        // Create all vertex and color VBO, enable them and draw in the windows
        virtual void draw(Shader shader) = 0;
};

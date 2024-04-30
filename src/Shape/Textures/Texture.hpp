#pragma once

#include "../../Lib.hpp"
#include <vector>

class Texture
{
    private:
        GLuint id;
        int width;
        int height;
        int channelsNumber;
        unsigned char *data;
        vector<vec2> texCoord;
        GLint textureMode;

    public:
        Texture() {}

        Texture(const char *path, vector<vec2> texCoord);

        void createTexture(bool pixelated = false);

        void createTexture(GLuint mode, bool pixelated);

        void setTextureMode(GLint mode) { this->textureMode = mode; }

        void setTexCoordsVector(vector<vec2> texCoords) { this->texCoord = texCoords; }

        vector<vec2> getTexCoordsVector() { return this->texCoord; }

        GLuint getId() { return this->id; }
};

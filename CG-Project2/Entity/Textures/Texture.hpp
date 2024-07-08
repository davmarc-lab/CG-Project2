#pragma once

#include "../../Lib.hpp"

class Texture
{
private:
    GLuint id;
    int width;
    int height;
    int channelsNumber;
    unsigned char *data;
    GLint textureMode;
    string path = "None";

public:
    Texture() {}

    Texture(const char *path);

    void createTexture(bool pixelated = false);

    void createTexture(GLuint mode, bool pixelated);

    inline void setTextureMode(GLint mode) { this->textureMode = mode; }

    inline GLuint getId() { return this->id; }

    inline const char* getPath() { return this->path.c_str(); }

    ~Texture() = default;
};

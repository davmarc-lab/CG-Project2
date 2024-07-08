#pragma once

#include "../Lib.hpp"
#include "../Shader/Shader.hpp"
#include "../Color/Color.hpp"

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID;     // ID handle of the glyph this->texture
    ivec2 Size;                 // Size of glyph
    ivec2 Bearing;              // Offset from baseline to left/top of glyph
    unsigned int Advance;       // Horizontal offset to advance to next glyph
};

struct TBuffers {
    GLuint vao;
    GLuint vbo;
    GLuint texture;
};

/*
 * This class create a text entity using freetype library.
 */
class Text {
private:

    TBuffers buffers;
    mat4 projection = ortho(0.f, (float)WIDTH, 0.f, (float)HEIGHT);
    unsigned int fontSize = 20;
    float fontScale = 1;

    FT_Library library;
    FT_Face fontFace;
    map<GLchar, Character> characters;

    string text;
    vec2 position = vec2(0, 0);
    Color color = Color(vec4(0));

    float totalWidth = 0;
    float totalHeight = 0;

    // Prepare the text for rendering.
    void generateText();

public:
    // Constructs an objetc with given projection, text and font size.
    Text(string text, const int fontSize);

    Text();

    // Creates and initialize buffers.
    void createVertexArray();

    // Renders the text given before.
    void renderText(Shader shader);

    // Sets the text position.
    inline void setPosition(vec2 position) { this->position = position; }

    // Retrieves the text position.
    inline vec2 getPosition() { return this->position; }

    // Retrieves text total width.
    inline float getTotalWidth() { return this->totalWidth; }

    // Retrieves text total height.
    inline float getTotalHeight() { return this->totalHeight; }

    // Sets the color of the text.
    inline void setColor(Color color) { this->color = color; }

    // Retrieves the text color vec4.
    inline vec4 getColorValues() { return this->color.getColorVector(); }

    // Sets the text.
    inline void setText(string text) { this->text = text; }

    // Add text at the current text.
    inline void appendText(string text) { this->text.append(text); }

    // Retrieves the text.
    inline string getText() {return this->text; }

    inline void setFontSize(const unsigned int size) { this->fontSize = size; }

    void clear();
};

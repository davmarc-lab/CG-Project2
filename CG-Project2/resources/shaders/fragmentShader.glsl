#version 330 core

in vec4 vertexColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform int color;
uniform sampler2D ourTexture;

void main()
{
    if (color == 1) {
        FragColor = vertexColor;
    } else {
        FragColor = texture(ourTexture, TexCoord);
    }
}

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

// Color of each vertex.
out vec4 vertexColor;
// Texture Coordinates.
out vec2 TexCoord;
// Normal vector
out vec3 Normal;
// 
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    FragPos = vec3(model * vec4(aPos, 1.0));
    vertexColor = aColor;
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
}

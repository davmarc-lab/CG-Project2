#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

layout (location = 4) in vec4 aColorOffset;
layout (location = 5) in mat4 aModelOffset;

out VS_OUT {
    vec4 vertColor;
    vec3 normal;
    vec2 texCoord;
} vs_out;

layout(std140, binding = 0) uniform Matrices {
    mat4 viewProj;
};

void main() {
    gl_Position = viewProj * aModelOffset * vec4(aPos, 1.f);
    vs_out.vertColor = aColorOffset;
    vs_out.normal = mat3(transpose(inverse(aModelOffset))) * aNormal;
    vs_out.texCoord = aTexCoord;
}

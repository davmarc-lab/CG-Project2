#version 420 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoord;

out VS_OUT {
    vec4 vertColor;
    vec3 normal;
    vec2 texCoord;
} vs_out;

uniform vec3 viewPos;

uniform mat4 model;

out vec3 FragPos;

layout(std140, binding = 0) uniform Matrices {
    mat4 viewProj;
};

void main() {
    FragPos = vec3(model * vec4(aPos, 1.f));
    gl_Position = viewProj * vec4(FragPos, 1.f);
    vs_out.vertColor = aColor;
    vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.texCoord = aTexCoord;
}

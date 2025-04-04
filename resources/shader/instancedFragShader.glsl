#version 420 core

in VS_OUT {
    vec4 vertColor;
    vec3 normal;
    vec2 texCoord;
} fs_out;

out vec4 FragColor;

void main() {
    FragColor = fs_out.vertColor;
}

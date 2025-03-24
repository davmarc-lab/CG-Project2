#version 420 core

in VS_OUT {
    vec4 vertColor;
    vec3 normal;
    vec2 texCoord;
} fs_out;

out vec4 fragColor;

uniform sampler2D texture_diffuse1;

void main() {
    fragColor = texture(texture_diffuse1, fs_out.texCoord);
}

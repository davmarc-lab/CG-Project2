#version 420 core

in VS_OUT {
    vec4 vertColor;
    vec3 normal;
    vec2 texCoord;
} fs_out;

out vec4 fragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;

uniform int current;

void main() {
    if (current == 1) {
        fragColor = texture(texture_diffuse1, fs_out.texCoord);
    } else if (current == 2) {
        fragColor = texture(texture_diffuse2, fs_out.texCoord);
    } else {
        fragColor = texture(texture_diffuse3, fs_out.texCoord);
    }
}

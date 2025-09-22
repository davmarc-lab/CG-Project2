#version 420 core

#define MAX_LIGHTS 32

in VS_OUT {
    vec4 vertColor;
    vec3 normal;
    vec2 texCoord;
} fs_out;

out vec4 fragColor;
in vec3 FragPos;

uniform vec3 camPos;

struct Light {
    int type;
    float intensity;
    vec3 color;

    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutoff;

    bool isSmooth;
};

uniform Light lights[MAX_LIGHTS];
uniform int lightsCount;

void main() {
    fragColor = fs_out.vertColor;

    vec3 N = normalize(fs_out.normal);
    vec3 V = normalize(camPos - FragPos);

    if (lightsCount > 0) {
        vec3 Lo = vec3(0.f);
        for (int i = 0; i < lightsCount && i < MAX_LIGHTS; i++) {
            vec3 L = normalize(lights[i].position - FragPos);
            vec3 H = normalize(V + L);

            float distance = length(lights[i].position - FragPos);
            float attenuation = 1.0 / (distance * distance);
            vec3 radiance = lights[i].color * attenuation;
        }
    }
}

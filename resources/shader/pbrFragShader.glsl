#version 420 core

#define MAX_LIGHTS 32

#define PI 3.14159265359

in VS_OUT {
    vec4 vertColor;
    vec3 normal;
    vec2 texCoord;
} fs_out;

out vec4 fragColor;
in vec3 FragPos;

struct PBMaterial {
    vec3 albedo;
    float roughness;
    float metallic;
    float ao;
};

uniform PBMaterial material;

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

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.f - F0) * pow(clamp(1.f - cosTheta, 0.f, 1.f), 5.f);
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main() {
    if (lightsCount > 0) {
        vec3 N = normalize(fs_out.normal);
        vec3 V = normalize(camPos - FragPos);

        vec3 F0 = vec3(0.04f);
        F0 = mix(F0, material.albedo, material.metallic);

        vec3 Lo = vec3(0.f);
        for (int i = 0; i < lightsCount && i < MAX_LIGHTS; i++) {
            vec3 L = normalize(lights[i].position - FragPos);
            vec3 H = normalize(V + L);

            float distance = length(lights[i].position - FragPos);
            float attenuation = 1.0 / (distance * distance);
            vec3 radiance = lights[i].color * attenuation;

            float NDF = distributionGGX(N, H, material.roughness);
            float G = GeometrySmith(N, V, L, material.roughness);
            vec3 F = fresnelSchlick(max(dot(H, V), 0.f), F0);

            vec3 kd = vec3(1.f) - F;
            kd *= 1.f - material.metallic;

            vec3 numerator = NDF * G * F;
            float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
            vec3 specular = numerator / denominator;

            float NdotL = max(dot(N, L), 0.0);
            Lo += (kd * material.albedo / PI + specular) * radiance * NdotL;
        }

        vec3 ambient = vec3(0.03) * material.albedo * material.ao;
        vec3 color = ambient + Lo;

        color = color / (color + vec3(1.0));
        // gamma correct
        color = pow(color, vec3(1.0/2.2));

        fragColor = vec4(color, 1.f);
    } else {
        fragColor = fs_out.vertColor;
    }
}

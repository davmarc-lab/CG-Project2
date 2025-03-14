#version 420 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoord;

#define MAX_LIGHTS 32

out VS_OUT {
    vec4 vertColor;
    vec3 normal;
    vec2 texCoord;
} vs_out;

// Materials structure
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Light structure
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

uniform Material material;
uniform vec3 viewPos;
uniform sampler2D texture1;

uniform int lightComp;

// Light parameters
uniform int lightsCount;
uniform Light lights[MAX_LIGHTS];

uniform mat4 model;

out vec3 FragPos;
out vec3 resIntShading;

vec3 norm = vec3(0);
vec3 lightDir = vec3(0);
float diff = 0.f;
vec3 viewDir = vec3(0);
vec3 reflectDir = vec3(0);
float spec = 0.f;

layout(std140, binding = 0) uniform Matrices {
    mat4 viewProj;
};

vec3 calcDirectionalLight(Light light) {
    // ambient
    vec3 ambient = light.intensity * light.color * light.ambient * material.ambient;

    // diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.intensity * light.color * light.diffuse * diff * material.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.intensity * light.color * light.specular * spec * material.specular;

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(Light light) {
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient = light.intensity * light.color * light.ambient * material.ambient;
    vec3 diffuse = light.intensity * light.color * light.diffuse * diff * material.diffuse;
    vec3 specular = light.intensity * light.color * light.specular * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(Light light) {
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = light.intensity * light.color * light.ambient * material.ambient;
    vec3 diffuse = light.intensity * light.color * light.diffuse * diff * material.diffuse;
    vec3 specular = light.intensity * light.color * light.specular * spec * material.specular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

void main() {
    FragPos = vec3(model * vec4(aPos, 1.f));
    gl_Position = viewProj * model * vec4(aPos, 1.f);
    vs_out.vertColor = aColor;
    vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.texCoord = aTexCoord;

    resIntShading = vec3(0);
    if (lightsCount >= 1) {
        // interpolative shader
        if (lightComp >= 3) {
            norm = normalize(vs_out.normal);
            viewDir = normalize(viewPos - FragPos);

            // calc multiple lights
            for (int i = 0; i < lightsCount && i < MAX_LIGHTS; i++) {
                lightDir = normalize(lights[i].position - FragPos);
                diff = max(dot(norm, lightDir), 0.0);
                reflectDir = lightComp == 4 ? normalize(lightDir + viewDir) : reflect(-lightDir, norm);
                spec = pow(max(lightComp == 4 ? dot(norm, reflectDir) : dot(viewDir, reflectDir), 0.0), material.shininess);

                switch (lights[i].type) {
                    case 0:
                    resIntShading += calcDirectionalLight(lights[i]);
                    break;
                    case 1:
                    resIntShading += calcPointLight(lights[i]);
                    break;
                    case 2:
                    resIntShading += calcSpotLight(lights[i]);
                    break;
                }
            }
        }
    }
}

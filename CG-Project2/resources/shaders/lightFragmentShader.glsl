#version 330 core

#define MAX_LIGHTS 32

// Materials structure
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Light structure
struct Light {
    int lightType;
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

    float cutOff;
    float outer_cutOff;
    bool isSmooth;
};

in vec2 TexCoord;
in vec3 FragPos;

// Color of each vertex
out vec4 FragColor;

// Texture sampler
uniform sampler2D ourTexture;

// Normal vector
in vec3 Normal;

// Light computation
uniform int light_comp;

// Light parameters
uniform int max_num_lights;
uniform int num_lights;

uniform vec3 viewPos;

uniform Material material;
uniform Light lights[MAX_LIGHTS];

in vec3 res_int_shading;

vec3 norm = vec3(0);
vec3 lightDir = vec3(0);
float diff = 0.f;
vec3 viewDir = vec3(0);
vec3 reflectDir = vec3(0);
float spec = 0.f;

vec3 calcDirectionalLight(Light light) {
    vec3 norm = normalize(Normal);

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
    float epsilon = light.cutOff - light.outer_cutOff;
    float intensity = clamp((theta - light.outer_cutOff) / epsilon, 0.0, 1.0);

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
    vec3 result = vec3(0);
    norm = normalize(Normal);
    viewDir = normalize(viewPos - FragPos);

    if (light_comp == 0 || light_comp == 1) {
        // phong shading
        if (max_num_lights >= 1) {
            // calc multiple lights
            for (int i = 0; i < num_lights && i < max_num_lights; i++) {
                lightDir = normalize(lights[i].position - FragPos);
                diff = max(dot(norm, lightDir), 0.0);
                reflectDir = light_comp == 1 ? normalize(lightDir + viewDir) : reflect(-lightDir, norm);
                spec = pow(max(light_comp == 1 ? dot(norm, reflectDir) : dot(viewDir, reflectDir), 0.0), material.shininess);

                switch (lights[i].lightType) {
                    case 0:
                    result += calcDirectionalLight(lights[i]);
                    break;
                    case 1:
                    result += calcPointLight(lights[i]);
                    break;
                    case 2:
                    result += calcSpotLight(lights[i]);
                    break;
                }
            }
        }
    } else if (light_comp == 2 || light_comp == 3) {
        // interpolative shading
        result = res_int_shading;
    } else {
        result = vec3(1);
    }

    FragColor = vec4(result * texture(ourTexture, TexCoord).rgb, 1.0);
}

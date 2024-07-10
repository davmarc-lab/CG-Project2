#version 330 core

// Materials structure
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Light structure
struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
    bool isSmooth;
};

in vec2 TexCoord;
in vec3 FragPos;

// Color of each vertex.
out vec4 FragColor;
// Texture sampler.
uniform sampler2D ourTexture;
// Normal vector.
in vec3 Normal;

// Light parameters
uniform int lightType;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

vec3 execPhongAlgorithm() {
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir;

    if (lightType == 0) // calculating direcitonal light
    {
        lightDir = normalize(-light.direction);
    }
    else {
        lightDir = normalize(light.position - FragPos);
    }
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * material.diffuse * diff;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.shininess * spec;

    return texture(ourTexture, TexCoord).rgb * (ambient + diffuse + specular);
}

vec3 calcDirectionalLight() {
    vec3 norm = normalize(Normal);

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    return texture(ourTexture, TexCoord).rgb * (ambient + diffuse + specular);
}

vec3 calcPointLight() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return texture(ourTexture, TexCoord).rgb * (ambient + diffuse + specular);
}

vec3 calcSpotLight() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return texture(ourTexture, TexCoord).rgb * (ambient + diffuse + specular);
}

void main()
{
    vec3 result;

    switch (lightType) {
        case 0:
        result = calcDirectionalLight();
        break;
        case 1:
        result = calcPointLight();
        break;
        case 2:
        result = calcSpotLight();
        break;
    }
    FragColor = vec4(result, 1.0);
}

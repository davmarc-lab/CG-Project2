#version 420 core

#define MAX_LIGHTS 32

in VS_OUT {
    vec4 vertColor;
    vec3 normal;
    vec2 texCoord;
} fs_out;

out vec4 fragColor;
in vec3 FragPos;
in vec3 resIntShading;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform vec3 viewPos;
uniform int lightComp;
uniform sampler2D texture1;
uniform samplerCube skybox;

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
uniform int reflective;
uniform int useColor;

vec3 norm = vec3(0);
vec3 lightDir = vec3(0);
float diff = 0.f;
vec3 viewDir = vec3(0);
vec3 reflectDir = vec3(0);
float spec = 0.f;

// type = 0
vec3 directionalLight(Light light) {
    vec3 norm = normalize(fs_out.normal);

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

// type = 1
vec3 pointLight(Light light) {
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

// type = 2
vec3 spotLight(Light light) {
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

    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

void main() {
    if (reflective != 0) {
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = reflect(I, normalize(fs_out.normal));
        fragColor = vec4(texture(skybox, R).rgb, 1);
        return;
    }

    // critical
    if (lightsCount > 0) {
        vec3 result = vec3(0);

        if (lightComp == 0) {
            fragColor = vec4(texture(texture1, fs_out.texCoord).rgb, 1);
        } else if (lightComp > 0 && lightComp < 3) {
            norm = normalize(fs_out.normal);
            viewDir = normalize(viewPos - FragPos);

            for (int i = 0; i < lightsCount; i++) {
                lightDir = normalize(lights[i].position - FragPos);
                diff = max(dot(norm, lightDir), 0.);
                reflectDir = lightComp == 2 ? normalize(lightDir + viewDir) : reflect(-lightDir, norm);
                spec = pow(max(lightComp == 2 ? dot(norm, reflectDir) : dot(viewDir, reflectDir), 0.0), material.shininess);

                // if (lights[i].type == 0) {
                //     result += directionalLight(lights[i]);
                // } else if (lights[i].type == 1) {
                //     result += pointLight(lights[i]);
                // } else if (lights[i].type == 2) {
                //     result += spotLight(lights[i]);
                // }

                switch (lights[i].type) {
                    case 0:
                    {
                        result += directionalLight(lights[i]);
                        break;
                    }
                    case 1:
                    {
                        result += pointLight(lights[i]);
                        break;
                    }
                    case 2:
                    {
                        result += spotLight(lights[i]);
                        break;
                    }
                }
            }
            if (useColor == 1) {
                fragColor = vec4(result * fs_out.vertColor.xyz, 1);
            } else {
                fragColor = vec4(result * texture(texture1, fs_out.texCoord).rgb, 1);
            }
        } else {
            // interpolative result
            fragColor = vec4(resIntShading * texture(texture1, fs_out.texCoord).rgb, 1);
        }
    } else {
        fragColor = fs_out.vertColor;
    }
}

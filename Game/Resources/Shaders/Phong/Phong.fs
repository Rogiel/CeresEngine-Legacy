#version 330 core

#define BLINN_PHONG 1

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in mat3 TBN;

out vec4 FragColor;

struct PhongMaterial {
    vec3 diffuseColor;
    sampler2D diffuse;

    vec3 specularColor;
    sampler2D specular;

    float shininess;

    bool hasNormalMap;
    sampler2D normal;

    bool hasDisplacementMap;
    sampler2D displacementMap;

    bool blinn;
};

layout(std140) uniform ViewProjection {
    mat4 projection;
    mat4 view;
    struct {
        vec3 position;
    } camera;
};

uniform vec3 cameraPosition;

#define MAX_LIGHTS 100
layout(std140) uniform Lights {
    struct DirectionalLights {
        int count;
        struct DirectionalLight {
            vec3 direction;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        } light[MAX_LIGHTS];
    } directional;

    struct PointLights {
        int count;
        struct PointLight {
            vec3 position;

            float constant;
            float linear;
            float quadratic;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        } light[MAX_LIGHTS];
    } point;

    struct SpotLights {
        int count;
        struct SpotLight {
            vec3 position;
            vec3 direction;
            float cutOff;
            float outerCutOff;

            float constant;
            float linear;
            float quadratic;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        } light[MAX_LIGHTS];
    } spot;
} lights;

uniform PhongMaterial material;
uniform float exposure;

// function prototypes
mat3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
mat3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // properties
    vec3 norm = normalize(TBN * Normal);
    if(material.hasNormalMap) {
        norm = normalize((2.0 * vec3(texture(material.normal, TexCoords)) - 1.0));
    }

    vec3 displacedFragPos = FragPos;
    if(material.hasDisplacementMap) {
        displacedFragPos += (2.0 * vec3(texture(material.displacementMap, TexCoords)) - 1.0);
    }
    displacedFragPos = TBN * displacedFragPos;

    vec3 viewDir = normalize(TBN * camera.position - TBN * FragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    mat3 result = mat3(0.0);

    // phase 1: directional lighting
    for(int i = 0; i < min(lights.directional.count, MAX_LIGHTS); i++) {
        result += CalcDirectionalLight(lights.directional.light[i], norm, viewDir);
    }

    // phase 2: point lights
    for(int i = 0; i < min(lights.point.count, MAX_LIGHTS); i++) {
        result += CalcPointLight(lights.point.light[i], norm, displacedFragPos, viewDir);
    }

    // phase 3: spot light
    for(int i = 0; i < min(lights.spot.count, MAX_LIGHTS); i++) {
        result += CalcSpotLight(lights.spot.light[i], norm, displacedFragPos, viewDir);
    }

    vec3 materialDiffuse = texture(material.diffuse, TexCoords).rgb + material.diffuseColor;
    vec3 materialSpecular = texture(material.specular, TexCoords).rgb + material.specularColor;
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;

    // apply some HDR magic...
    const float gamma = 2.2;

    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-color * exposure);
    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);

//    FragColor = vec4(color, 1.0);
}

// calculates the color when using a directional light.
mat3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float spec;
    if(material.blinn == false) {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    } else {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }

    // combine results
    return mat3(
        light.ambient,
        light.diffuse * diff,
        light.specular * spec
    );
}

// calculates the color when using a point light.
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightPos = TBN * light.position;
    vec3 lightDir = normalize(lightPos - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float spec;
    if(material.blinn == false) {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    } else {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0 * material.shininess);
    }

    // attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (/*light.constant + */light.linear * distance + light.quadratic * pow(distance, 2));

    // combine results
    return mat3(
        light.ambient * attenuation,
        light.diffuse * diff * attenuation,
        light.specular * spec * attenuation
    );
}

// calculates the color when using a spot light.
mat3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float spec;
    if(material.blinn == false) {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    } else {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    return mat3(
        light.ambient * attenuation * intensity,
        light.diffuse * diff * attenuation * intensity,
        light.specular * spec * attenuation * intensity
    );
}

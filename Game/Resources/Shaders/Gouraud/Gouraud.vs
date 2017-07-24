#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

// Output data ; will be interpolated for each fragment.
out vec2 TexCoords;
out vec3 AmbientShading;
out vec3 DiffuseShading;
out vec3 SpecularShading;

uniform mat4 model;
uniform mat4 inversedTransposedModel;
layout(std140) uniform ViewProjection {
    mat4 projection;
    mat4 view;
    struct {
        vec3 position;
    } camera;
};

uniform mat4 lightSpaceMatrix;

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
uniform PhongMaterial material;

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

// function prototypes
mat3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
mat3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * vec4(fragPos, 1.0);
    TexCoords = aTexCoords;

    vec3 normal = normalize(mat3(inversedTransposedModel) * aNormal);
    vec3 viewDir = normalize(camera.position - fragPos);

    mat3 result = mat3(0.0);
    // phase 1: directional lighting
//    for(int i = 0; i < min(lights.directional.count, MAX_LIGHTS); i++) {
//        result += CalcDirectionalLight(lights.directional.light[i], normal, viewDir);
//    }

    // phase 2: point lights
    for(int i = 0; i < min(lights.point.count, MAX_LIGHTS); i++) {
        result += CalcPointLight(lights.point.light[i], normal, fragPos, viewDir);
    }

    // phase 3: spot light
//    for(int i = 0; i < min(lights.spot.count, MAX_LIGHTS); i++) {
//        result += CalcSpotLight(lights.spot.light[i], normal, fragPos, viewDir);
//    }

    AmbientShading = result[0];
    DiffuseShading = result[1];
    SpecularShading = result[2];

//    VertexShading = result;
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
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float spec;
//    if(material.blinn == false) {
//        vec3 reflectDir = reflect(-lightDir, normal);
//        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    } else {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
//    }

    // attenuation
    float distance = length(light.position - fragPos);
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
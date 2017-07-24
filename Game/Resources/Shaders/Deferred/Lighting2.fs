#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

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

uniform int pointLightCount;

// function prototypes
mat3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, float shininess);
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess);
mat3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess);

#define MAX_RENDER_LIGHTS 10

void main() {
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    float Depth = texture(gPosition, TexCoords).a;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    float Shininess = texture(gNormal, TexCoords).a;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 viewDir = normalize(camera.position - FragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    mat3 result = mat3(0.0);

//    // phase 1: directional lighting
//    for(int i = 0; i < min(lights.directional.count, MAX_RENDER_LIGHTS); i++) {
//        result += CalcDirectionalLight(lights.directional.light[i], Normal, viewDir, Shininess);
//    }

    // phase 2: point lights
    for(int i = 0; i < pointLightCount; i++) {
        result += CalcPointLight(lights.point.light[i], Normal, FragPos, viewDir, Shininess);
    }

//    // phase 3: spot light
//    for(int i = 0; i < min(lights.spot.count, MAX_RENDER_LIGHTS); i++) {
//        result += CalcSpotLight(lights.spot.light[i], Normal, FragPos, viewDir, Shininess);
//    }

    vec3 materialDiffuse = Diffuse;
    vec3 materialSpecular = vec3(Specular);
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;

//    FragColor = vec4(color, 1.0);

    // apply some HDR magic...
    const float gamma = 2.2;
    const float exposure = 1.0;

    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-color * exposure);
    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}


// calculates the color when using a directional light.
mat3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, float shininess) {
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float spec;
//    if(material.blinn == false) {
//        vec3 reflectDir = reflect(-lightDir, normal);
//        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    } else {
//        vec3 halfwayDir = normalize(lightDir + viewDir);
//        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
//    }
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    // combine results
    return mat3(
        light.ambient,
        light.diffuse * diff,
        light.specular * spec
    );
}

// calculates the color when using a point light.
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess) {
    vec3 lightPos = light.position;
    vec3 lightDir = normalize(lightPos - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float spec;
//    if(material.blinn == false) {
//        vec3 reflectDir = reflect(-lightDir, normal);
//        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    } else {
//        vec3 halfwayDir = normalize(lightDir + viewDir);
//        spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0 * material.shininess);
//    }
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

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
mat3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess) {
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float spec;
    /*if(material.blinn == false) {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    } else {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }*/
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

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

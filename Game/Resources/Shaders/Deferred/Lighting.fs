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

uniform struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light;

// function prototypes
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess);

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
    // mat3 result = mat3(0.0);

//    // phase 1: directional lighting
//    for(int i = 0; i < min(lights.directional.count, MAX_RENDER_LIGHTS); i++) {
//        result += CalcDirectionalLight(lights.directional.light[i], Normal, viewDir, Shininess);
//    }

    mat3 result = CalcPointLight(light, Normal, FragPos, viewDir, Shininess);

//    // phase 2: point lights
//    for(int i = 0; i < pointLightCount; i++) {
//        result += CalcPointLight(lights.point.light[i], Normal, FragPos, viewDir, Shininess);
//    }

//    // phase 3: spot light
//    for(int i = 0; i < min(lights.spot.count, MAX_RENDER_LIGHTS); i++) {
//        result += CalcSpotLight(lights.spot.light[i], Normal, FragPos, viewDir, Shininess);
//    }

    vec3 materialDiffuse = Diffuse;
    vec3 materialSpecular = vec3(Specular);
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;

    FragColor = vec4(color, 1.0);
}

// calculates the color when using a point light.
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess) {
    vec3 lightPos = light.position;
    vec3 lightDir = normalize(lightPos - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
//    float spec;
//    if(material.blinn == false) {
//        vec3 reflectDir = reflect(-lightDir, normal);
//        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
//    } else {
//        vec3 halfwayDir = normalize(lightDir + viewDir);
//        spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0 * material.shininess);
//    }
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    // attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));

    // combine results
    return mat3(
        light.ambient * attenuation,
        light.diffuse * diff * attenuation,
        light.specular * spec * attenuation
    );
}

#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform float farPlane;

uniform bool hasShadowMap;
uniform samplerCube shadowMap;

uniform struct {
    vec3 position;
} camera;

#include "Shading/Phong/PointLight.glsl"
uniform PointLight light;

void main() {
    #include "DeferredRendering/SampleGBuffer.glsl"

    vec3 viewDir = normalize(camera.position - FragPos);
    // mat3 result = mat3(0.0);

    //for(int i = 0; i<LIGHT_COUNT; i++) {
    //     result += CalcPointLight(light, Normal, FragPos, viewDir, Shininess);
    //}

    mat3 result = CalcPointLight(light, Normal, FragPos, viewDir, Shininess, farPlane);

    vec3 materialDiffuse = Diffuse;
    vec3 materialSpecular = materialDiffuse * vec3(Specular);
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;
    // FragColor = vec4(FragPos / 6371000.0 / 2.0, 1.0);
    FragColor = vec4(color, 1.0);
}

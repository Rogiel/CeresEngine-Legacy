#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform struct {
    vec3 position;
} camera;

#include "Shading/Phong/DirectionalLight.glsl"
uniform DirectionalLight light;

void main() {
    #include "DeferredRendering/SampleGBuffer.glsl"

    vec3 viewDir = normalize(camera.position - FragPos);
    mat3 result = CalcDirectionalLight(light, Normal, viewDir, Shininess);

    vec3 materialDiffuse = Diffuse;
    vec3 materialSpecular = materialDiffuse * vec3(Specular);
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;

    FragColor = vec4(color, 1.0);
}

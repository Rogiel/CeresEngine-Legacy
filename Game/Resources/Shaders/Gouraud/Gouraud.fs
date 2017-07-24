#version 330 core

in vec2 TexCoords;
in vec3 AmbientShading;
in vec3 DiffuseShading;
in vec3 SpecularShading;

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
uniform PhongMaterial material;

void main() {
    // material diffuse and specular components
    vec3 materialDiffuse = vec3(texture(material.diffuse, TexCoords)) + material.diffuseColor;
    vec3 materialSpecular = vec3(texture(material.specular, TexCoords)) + material.specularColor;

    vec3 result = (AmbientShading + DiffuseShading) * materialDiffuse +
        SpecularShading * materialSpecular;

    // apply some HDR magic...
    const float exposure = 1.0;
    const float gamma = 2.2;

    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-result * exposure);
    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}

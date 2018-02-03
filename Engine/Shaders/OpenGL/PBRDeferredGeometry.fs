#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

layout (location = 0) out vec4 gPositionAndMetallic;
layout (location = 1) out vec4 gNormalAndRoughness;
layout (location = 2) out vec4 gAlbedoTextureAndAmbientOcclusion;

#include "Material/PBR.glsl"
uniform PBRMaterial material;

uniform float gamma = 2.2;
in float logz;

void main() {
    gl_FragDepth = logz;

    vec3 mappedNormal = texture(material.normalMap, TexCoords).xyz * 2.0 - 1.0;

    // write outputs
    gPositionAndMetallic = vec4(
        FragPos,
        texture(material.metallicMap, TexCoords).r
    );
    gNormalAndRoughness = vec4(
        normalize(Normal + TBN * mappedNormal),
        texture(material.roughnessMap, TexCoords).r
    );
    gAlbedoTextureAndAmbientOcclusion = vec4(
        pow(texture(material.albedoMap, TexCoords).rgb, vec3(gamma)),
        texture(material.ambientOcclusionMap, TexCoords).r
    );
}
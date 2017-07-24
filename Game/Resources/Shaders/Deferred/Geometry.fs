#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

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
    // store the fragment position vector in the first gbuffer texture
    gPosition.rgb = FragPos;
//    gPosition.a = gl_FragDepth;

    // also store the per-fragment normals into the gbuffer
    vec3 bump = texture(material.normal, TexCoords).xyz * 2.0 - 1.0;
    gNormal.rgb = normalize(TBN * bump);
    gNormal.a = material.shininess;

    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(material.diffuse, TexCoords).rgb + material.diffuseColor;

    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(material.specular, TexCoords).r + material.specularColor.r;
}
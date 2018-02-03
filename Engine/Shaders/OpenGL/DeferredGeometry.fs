#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

#include "Material/Phong.glsl"
uniform PhongMaterial material;

uniform float gamma = 2.2;
in float logz;

void main() {
    gl_FragDepth = logz;

    // store the fragment position vector in the first gbuffer texture
    gPosition = vec4(FragPos, 0.0);

    // also store the per-fragment normals into the gbuffer
    if(material.hasNormalMap) {
        vec3 normal = texture(material.normal, TexCoords).xyz * 2.0 - 1.0;
        if(gl_FrontFacing == false) {
            normal = -normal;
        }
        gNormal.rgb = normalize(TBN * normal);
    } else {
        gNormal.rgb = Normal;
    }
    gNormal.a = material.shininess;

    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = pow(texture(material.diffuse, TexCoords).rgb, vec3(gamma));
    if(material.hasEmissionMap) {
        gAlbedoSpec.rgb += material.emission * texture(material.emissionMap, TexCoords).rgb;
    }

    // store specular intensity in gAlbedoSpec's alpha component
    vec4 specular = texture(material.specular, TexCoords);
    gAlbedoSpec.a = specular.r;
}
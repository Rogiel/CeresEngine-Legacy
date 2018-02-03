#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out mat3 TBN;

uniform mat4 model;
uniform mat4 inversedTransposedModel;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 MVP;

const float C = 1;
out float logz;

#include "Material/PBR.glsl"
uniform PBRMaterial material;

#include "Object/Camera.glsl"
uniform Camera camera;

// http://outerra.blogspot.com.br/2013/07/logarithmic-depth-buffer-optimizations.html
// http://outerra.blogspot.com.br/2009/08/logarithmic-z-buffer.html
void main() {
    TexCoords = aTexCoords;
    Normal = normalize(mat3(inversedTransposedModel) * aNormal);

    vec3 T = vec3(normalize(inversedTransposedModel * vec4(aTangent, 0)));
    vec3 N = Normal;
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);


    vec3 displacement = vec3(0.0);
    if(material.maximumDisplacement != 0) {
        float displacementFactor = (texture(material.displacementMap, TexCoords).r * 2.0 - 1.0) * material.maximumDisplacement;
        displacement = vec3(normalize(aNormal) * vec3(displacementFactor));
    }
    gl_Position = MVP * vec4(aPos + displacement, 1.0);
    vec4 worldPos = model * vec4(aPos + displacement, 1.0);
    FragPos = worldPos.xyz;

    float FC = 1.0/log(camera.far*C + 1);
    logz = log(gl_Position.w * C + 1) * FC;
}
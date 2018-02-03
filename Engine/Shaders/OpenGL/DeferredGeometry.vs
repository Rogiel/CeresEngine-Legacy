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

uniform float near;
uniform float far;

const float C = 1;
out float logz;

// http://outerra.blogspot.com.br/2013/07/logarithmic-depth-buffer-optimizations.html
// http://outerra.blogspot.com.br/2009/08/logarithmic-z-buffer.html
void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    TexCoords = aTexCoords;
    gl_Position = MVP * vec4(aPos, 1.0);

    float FC = 1.0/log(far*C + 1);
    logz = log(gl_Position.w * C + 1) * FC;

    Normal = normalize(mat3(inversedTransposedModel) * aNormal);
    /*
    vec3 T = vec3(normalize(vec4(aTangent, 0)));
    T = normalize(T - dot(T, Normal) * Normal);
    vec3 B = cross(Normal, T);

    TBN = mat3(T, B, Normal);
    */
    vec3 T = vec3(normalize(inversedTransposedModel * vec4(aTangent, 0)));
    vec3 N = Normal;
//    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);
}
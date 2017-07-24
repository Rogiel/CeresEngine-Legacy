#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBinormal;

// Output data ; will be interpolated for each fragment.
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

out mat3 TBN;

uniform mat4 model;
uniform mat4 inversedTransposedModel;
layout(std140) uniform ViewProjection {
    mat4 projection;
    mat4 view;
    struct {
        vec3 position;
    } camera;
};

uniform mat4 lightSpaceMatrix;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    Normal = mat3(inversedTransposedModel) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);

    vec3 T = vec3(normalize(inversedTransposedModel * vec4(aTangent, 0)));
    vec3 N = vec3(normalize(inversedTransposedModel * vec4(aNormal, 0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    TBN = transpose(mat3(T, B, N));
}
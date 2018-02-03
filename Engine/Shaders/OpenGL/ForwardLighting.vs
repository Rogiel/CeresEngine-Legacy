#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform struct {
    vec3 position;
} camera;
uniform mat4 MVP;

out vec2 TexCoord;

uniform float near;
uniform float far;

const float C = 0.01;
out float logz;

void main() {
    gl_Position = MVP * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;

    float FC = 1.0/log(far*C + 1);
    logz = log(gl_Position.w * C + 1) * FC;

    gl_Position.z = 2.0 * log(gl_Position.w * C + 1)/log(far * C + 1) - 1;
    gl_Position.z *= gl_Position.w;
}

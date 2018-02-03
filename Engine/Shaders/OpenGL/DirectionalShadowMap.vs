#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

uniform float far;
uniform float near;

const float C = 0.01;

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);

    gl_Position.z = 2.0 * log(gl_Position.w * C + 1)/log(far * C + 1) - 1;
    gl_Position.z *= gl_Position.w;
}
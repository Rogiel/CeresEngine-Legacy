#version 330 core

in vec3  TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;
uniform float gamma = 2.2;

void main() {
    FragColor = pow(texture(skybox, TexCoords), vec4(gamma));
}
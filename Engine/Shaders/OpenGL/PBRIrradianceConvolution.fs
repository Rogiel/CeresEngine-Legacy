#version 330 core

#include "Shading/PBR/ImageBasedLighting.glsl"

out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main() {
    FragColor = vec4(
        IrradianceConvolution(WorldPos, environmentMap),
        1.0
    );
}

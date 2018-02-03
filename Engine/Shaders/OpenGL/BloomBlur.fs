#version 330 core

/// This shader blurs the entire image.

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D scene;
uniform bool horizontal;

#include "Effect/Blur.glsl"

void main() {
    vec2 tex_offset = textureSize(scene, 0); // gets size of single texel
    if(horizontal) {
        FragColor = blur13(scene, TexCoords, tex_offset, vec2(1.0, 0.0));
    } else {
        FragColor = blur13(scene, TexCoords, tex_offset, vec2(0.0, 1.0));
    }
}
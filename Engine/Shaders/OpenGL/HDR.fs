#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloom;

uniform bool hasHDR     = true;
uniform bool hasBloom   = true;

uniform float exposure  = 1.0;
uniform float gamma     = 2.2;

void main() {
    // fetch the scene color
    vec3 color = texture(scene, TexCoords).rgb;
    if(hasBloom) {
        color += texture(bloom, TexCoords).rgb;
    }

    // tone mapping, if enabled
    if(hasHDR) {
        color = vec3(1.0) - exp(-color * exposure);
    }
    FragColor = vec4(pow(color, vec3(1.0 / gamma)), 1.0);
}
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform struct {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
} material;

in float logz;

void main() {
    // FragColor = vec4(TexCoord.x, TexCoord.y, 1.0, 1.0);
    gl_FragDepth = logz;
    FragColor = texture(material.diffuse, TexCoord);
}
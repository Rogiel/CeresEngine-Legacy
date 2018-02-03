
//
//  BloomBlurShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The Bloom Blur vertex shader source
    public static var bloomBlurVertexShaderSource: String { return """
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}

""" }


    /// This shader blurs the entire image.
    public static var bloomBlurFragmentShaderSource: String { return """
#version 330 core

/// This shader blurs the entire image.

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D scene;
uniform bool horizontal;
vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
    vec4 color = vec4(0.0);
    vec2 off1 = vec2(1.411764705882353) * direction;
    vec2 off2 = vec2(3.2941176470588234) * direction;
    vec2 off3 = vec2(5.176470588235294) * direction;
    color += texture(image, uv) * 0.1964825501511404;
    color += texture(image, uv + (off1 / resolution)) * 0.2969069646728344;
    color += texture(image, uv - (off1 / resolution)) * 0.2969069646728344;
    color += texture(image, uv + (off2 / resolution)) * 0.09447039785044732;
    color += texture(image, uv - (off2 / resolution)) * 0.09447039785044732;
    color += texture(image, uv + (off3 / resolution)) * 0.010381362401148057;
    color += texture(image, uv - (off3 / resolution)) * 0.010381362401148057;
    return color;
}
void main() {
    vec2 tex_offset = textureSize(scene, 0); // gets size of single texel
    if(horizontal) {
        FragColor = blur13(scene, TexCoords, tex_offset, vec2(1.0, 0.0));
    } else {
        FragColor = blur13(scene, TexCoords, tex_offset, vec2(0.0, 1.0));
    }
}

""" }

}


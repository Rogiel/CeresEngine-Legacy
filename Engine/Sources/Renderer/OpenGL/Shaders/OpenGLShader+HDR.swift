
//
//  HDRShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The HDR vertex shader source
    public static var hdrVertexShaderSource: String { return """
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}

""" }


    /// The HDR fragment shader source
    public static var hdrFragmentShaderSource: String { return """
#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloom;

uniform bool hasHDR = true;
uniform bool hasBloom = true;

uniform float exposure = 1.0;
uniform float gamma = 2.2;

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

""" }

}


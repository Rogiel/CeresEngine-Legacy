
//
//  BloomExposureMappingShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The Bloom Exposure Mapping vertex shader source
    public static var bloomExposureMappingVertexShaderSource: String { return """
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}

""" }


    /// The Bloom Exposure Mapping fragment shader source
    public static var bloomExposureMappingFragmentShaderSource: String { return """
#version 330 core

in vec2 TexCoords;
out vec4 BloomColor;

uniform sampler2D scene;

void main() {
    vec3 fragColor = texture(scene, TexCoords).rgb;

    // check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(fragColor, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        BloomColor = vec4(vec3(brightness), 1.0);
    } else {
        BloomColor = vec4(0.0);
    }
}

""" }

}


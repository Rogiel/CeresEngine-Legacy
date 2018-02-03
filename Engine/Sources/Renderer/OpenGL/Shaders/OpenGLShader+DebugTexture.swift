
//
//  DebugTextureShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The Debug Texture vertex shader source
    public static var debugTextureVertexShaderSource: String { return """
#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;

out vec2 TexCoords;

uniform mat4 model;

void main() {
    gl_Position = model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}

""" }


    /// The Debug Texture fragment shader source
    public static var debugTextureFragmentShaderSource: String { return """
#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;

void main() {
    FragColor = texture(image, TexCoords);
}

""" }

}


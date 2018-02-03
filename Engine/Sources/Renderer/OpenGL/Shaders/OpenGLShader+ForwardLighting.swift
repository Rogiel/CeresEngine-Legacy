
//
//  ForwardLightingShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The Forward Lighting vertex shader source
    public static var forwardLightingVertexShaderSource: String { return """
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform struct {
    vec3 position;
} camera;
uniform mat4 MVP;

out vec2 TexCoord;

uniform float near;
uniform float far;

const float C = 0.01;
out float logz;

void main() {
    gl_Position = MVP * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;

    float FC = 1.0/log(far*C + 1);
    logz = log(gl_Position.w * C + 1) * FC;

    gl_Position.z = 2.0 * log(gl_Position.w * C + 1)/log(far * C + 1) - 1;
    gl_Position.z *= gl_Position.w;
}

""" }


    /// The Forward Lighting fragment shader source
    public static var forwardLightingFragmentShaderSource: String { return """
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

""" }

}


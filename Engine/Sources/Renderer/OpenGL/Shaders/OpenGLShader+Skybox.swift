
//
//  SkyboxShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The Skybox vertex shader source
    public static var skyboxVertexShaderSource: String { return """
#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aPos;
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
    gl_Position = pos.xyww;

}

""" }


    /// The Skybox fragment shader source
    public static var skyboxFragmentShaderSource: String { return """
#version 330 core

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;
uniform float gamma = 2.2;

void main() {
    FragColor = pow(texture(skybox, TexCoords), vec4(gamma));
}

""" }

}


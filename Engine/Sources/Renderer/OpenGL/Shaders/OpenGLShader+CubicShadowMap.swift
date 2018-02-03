
//
//  CubicShadowMapShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The Cubic Shadow Map vertex shader source
    public static var cubicShadowMapVertexShaderSource: String { return """
#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 FragPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

uniform float far;
uniform float near;

const float C = 0.01;

void main() {
    FragPos = model * vec4(aPos, 1.0);
    gl_Position = lightSpaceMatrix * FragPos;
    //gl_Position.z = 2.0 * log(gl_Position.w * C + 1)/log(far * C + 1) - 1;
    //gl_Position.z *= gl_Position.w;
}

""" }


    /// The Cubic Shadow Map fragment shader source
    public static var cubicShadowMapFragmentShaderSource: String { return """
#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() {
    float lightDistance = length(FragPos.xyz - lightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / farPlane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}

""" }

}


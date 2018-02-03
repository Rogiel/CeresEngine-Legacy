
//
//  DeferredDirectionalLightShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The Deferred Directional Light vertex shader source
    public static var deferredDirectionalLightVertexShaderSource: String { return """
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

""" }


    /// The Deferred Directional Light fragment shader source
    public static var deferredDirectionalLightFragmentShaderSource: String { return """
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform struct {
    vec3 position;
} camera;
struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shadowOcclusionStrength;
};
// calculates the color when using a directional light.
mat3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, float shininess) {
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    // combine results
    return mat3(
        light.ambient,
        light.diffuse * diff,
        light.specular * spec
    );
}
uniform DirectionalLight light;

void main() {
// retrieve data from gbuffer
vec3 FragPos = texture(gPosition, TexCoords).rgb;

float Depth = texture(gPosition, TexCoords).a;
vec3 Normal = texture(gNormal, TexCoords).rgb;
float Shininess = texture(gNormal, TexCoords).a;
vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
float Specular = texture(gAlbedoSpec, TexCoords).a;
    vec3 viewDir = normalize(camera.position - FragPos);
    mat3 result = CalcDirectionalLight(light, Normal, viewDir, Shininess);

    vec3 materialDiffuse = Diffuse;
    vec3 materialSpecular = materialDiffuse * vec3(Specular);
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;

    FragColor = vec4(color, 1.0);
}

""" }

}


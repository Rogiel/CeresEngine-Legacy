
//
//  DeferredPointLightShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The Deferred Point Light vertex shader source
    public static var deferredPointLightVertexShaderSource: String { return """
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

""" }


    /// The Deferred Point Light fragment shader source
    public static var deferredPointLightFragmentShaderSource: String { return """
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform float farPlane;

uniform bool hasShadowMap;
uniform samplerCube shadowMap;

uniform struct {
    vec3 position;
} camera;
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shadowOcclusionStrength;
};
float ShadowCalculation(vec3 fragPos, vec3 lightPos, vec3 viewPos, float shadowOcclusionStrength);

// calculates the color when using a point light.
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess, float farPlane) {
    vec3 lightPos = light.position;
    vec3 lightDir = normalize(lightPos - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    //vec3 halfwayDir = normalize(lightDir + viewDir);
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    // attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));

    // compute the shadow visibility factor
    float visibility = 1.0;
    if(hasShadowMap) {
        // visibility = ShadowCalculation(fragPos, lightPos, camera.position, light.shadowOcclusionStrength);
    }

    // combine results
    return mat3(
        light.ambient,
        light.diffuse * diff * attenuation * visibility,
        light.specular * spec * attenuation * visibility
    );
}

vec3 sampleOffsetDirections[20] = vec3[](
   vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
   vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
   vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
   vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos, vec3 lightPos, vec3 viewPos, float shadowOcclusionStrength) {
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;

    // ise the fragment to light vector to sample from the depth map
    float closestDepth = texture(shadowMap, fragToLight).r;

    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= farPlane;

    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    // test for shadows
    const float bias = 0.005 * 2;
    float shadow = currentDepth - bias > closestDepth ? (1.0 - shadowOcclusionStrength) : 1.0;

//    float shadow = 0.0;
//    const float bias   = 0.15;
//    const int samples  = 2;
//    float viewDistance = length(viewPos - fragPos);
//    const float diskRadius = 0.05;
//    int i = 0;
////    for(int i = 0; i < 2; i++) {
//        float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r * farPlane;
//        shadow += currentDepth - bias > closestDepth ? 0.0 : 1.0;
////    }
//    shadow /= float(samples);

    return shadow;
}
uniform PointLight light;

void main() {
// retrieve data from gbuffer
vec3 FragPos = texture(gPosition, TexCoords).rgb;

float Depth = texture(gPosition, TexCoords).a;
vec3 Normal = texture(gNormal, TexCoords).rgb;
float Shininess = texture(gNormal, TexCoords).a;
vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
float Specular = texture(gAlbedoSpec, TexCoords).a;
    vec3 viewDir = normalize(camera.position - FragPos);
    // mat3 result = mat3(0.0);

    //for(int i = 0; i<LIGHT_COUNT; i++) {
    //     result += CalcPointLight(light, Normal, FragPos, viewDir, Shininess);
    //}

    mat3 result = CalcPointLight(light, Normal, FragPos, viewDir, Shininess, farPlane);

    vec3 materialDiffuse = Diffuse;
    vec3 materialSpecular = materialDiffuse * vec3(Specular);
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;
    // FragColor = vec4(FragPos / 6371000.0 / 2.0, 1.0);
    FragColor = vec4(color, 1.0);
}

""" }

}


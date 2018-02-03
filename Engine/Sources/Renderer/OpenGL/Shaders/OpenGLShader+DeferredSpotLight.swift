
//
//  DeferredSpotLightShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The Deferred Spot Light vertex shader source
    public static var deferredSpotLightVertexShaderSource: String { return """
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

""" }


    /// The Deferred Spot Light fragment shader source
    public static var deferredSpotLightFragmentShaderSource: String { return """
#version 330

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D shadowMap;

uniform bool hasShadowMap;
uniform mat4 lightSpaceMatrix;
uniform struct {
    vec3 position;
} camera;
struct SpotLight {
    vec3 position;
    vec3 direction;

    /// Spotlight cutoff angle
    float cutOff;

    /// Spotlight outer cutoff angle
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shadowOcclusionStrength;
};
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, float shadowOcclusionStrength);

// calculates the color when using a spot light.
mat3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess) {
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // shadows
    float visibility = 1.0;
    if(hasShadowMap) {
        visibility = ShadowCalculation(lightSpaceMatrix * vec4(fragPos, 1.0), normal, lightDir, light.shadowOcclusionStrength);
    }

    // combine results
    return mat3(
        light.ambient * attenuation * intensity,
        light.diffuse * diff * attenuation * intensity * visibility,
        light.specular * spec * attenuation * intensity * visibility
    );
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, float shadowOcclusionStrength) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // if outside of shadow map bounds, return no shadows
    if(projCoords.z > 1.0)
        return 1.0;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // compute the bias factor based on the fragment distance from the light
    float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005);
//    const float bias = 0.0;

    // apply a PCSS technique to get a bit better shadows
    const int smoothing = 2;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -smoothing; x <= smoothing; ++x) {
        for(int y = -smoothing; y <= smoothing; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? (1.0 - shadowOcclusionStrength) : 1.0;
        }
    }
    shadow /= pow(2.0 * float(smoothing) + 1.0, 2);

    // return the shadow to the caller
    return shadow;
}
uniform SpotLight light;

void main() {
// retrieve data from gbuffer
vec3 FragPos = texture(gPosition, TexCoords).rgb;

float Depth = texture(gPosition, TexCoords).a;
vec3 Normal = texture(gNormal, TexCoords).rgb;
float Shininess = texture(gNormal, TexCoords).a;
vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
float Specular = texture(gAlbedoSpec, TexCoords).a;
    vec3 viewDir = normalize(camera.position - FragPos);
    mat3 result = CalcSpotLight(light, Normal, FragPos, viewDir, Shininess);

    vec3 materialDiffuse = Diffuse;
    vec3 materialSpecular = materialDiffuse * vec3(Specular);
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;

    FragColor = vec4(color, 1.0);
}

""" }

}


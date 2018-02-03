
//
//  PBRDeferredGeometryShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The PBR Deferred Geometry vertex shader source
    public static var pbrDeferredGeometryVertexShaderSource: String { return """
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out mat3 TBN;

uniform mat4 model;
uniform mat4 inversedTransposedModel;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 MVP;

const float C = 1;
out float logz;
struct PBRMaterial {
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D ambientOcclusionMap;

    sampler2D displacementMap;
    float maximumDisplacement;

    sampler2D emissionMap;
};
uniform PBRMaterial material;
struct Camera {
    vec3 position;
    float near;
    float far;
};
uniform Camera camera;

// http://outerra.blogspot.com.br/2013/07/logarithmic-depth-buffer-optimizations.html
// http://outerra.blogspot.com.br/2009/08/logarithmic-z-buffer.html
void main() {
    TexCoords = aTexCoords;
    Normal = normalize(mat3(inversedTransposedModel) * aNormal);

    vec3 T = vec3(normalize(inversedTransposedModel * vec4(aTangent, 0)));
    vec3 N = Normal;
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);


    vec3 displacement = vec3(0.0);
    if(material.maximumDisplacement != 0) {
        float displacementFactor = (texture(material.displacementMap, TexCoords).r * 2.0 - 1.0) * material.maximumDisplacement;
        displacement = vec3(normalize(aNormal) * vec3(displacementFactor));
    }
    gl_Position = MVP * vec4(aPos + displacement, 1.0);
    vec4 worldPos = model * vec4(aPos + displacement, 1.0);
    FragPos = worldPos.xyz;

    float FC = 1.0/log(camera.far*C + 1);
    logz = log(gl_Position.w * C + 1) * FC;
}

""" }


    /// The PBR Deferred Geometry fragment shader source
    public static var pbrDeferredGeometryFragmentShaderSource: String { return """
#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

layout (location = 0) out vec4 gPositionAndMetallic;
layout (location = 1) out vec4 gNormalAndRoughness;
layout (location = 2) out vec4 gAlbedoTextureAndAmbientOcclusion;
struct PBRMaterial {
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D ambientOcclusionMap;

    sampler2D displacementMap;
    float maximumDisplacement;

    sampler2D emissionMap;
};
uniform PBRMaterial material;

uniform float gamma = 2.2;
in float logz;

void main() {
    gl_FragDepth = logz;

    vec3 mappedNormal = texture(material.normalMap, TexCoords).xyz * 2.0 - 1.0;

    // write outputs
    gPositionAndMetallic = vec4(
        FragPos,
        texture(material.metallicMap, TexCoords).r
    );
    gNormalAndRoughness = vec4(
        normalize(Normal + TBN * mappedNormal),
        texture(material.roughnessMap, TexCoords).r
    );
    gAlbedoTextureAndAmbientOcclusion = vec4(
        pow(texture(material.albedoMap, TexCoords).rgb, vec3(gamma)),
        texture(material.ambientOcclusionMap, TexCoords).r
    );
}

""" }

}


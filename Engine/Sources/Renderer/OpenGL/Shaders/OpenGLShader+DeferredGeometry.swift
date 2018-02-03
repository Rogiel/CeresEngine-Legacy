
//
//  DeferredGeometryShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The Deferred Geometry vertex shader source
    public static var deferredGeometryVertexShaderSource: String { return """
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

uniform float near;
uniform float far;

const float C = 1;
out float logz;

// http://outerra.blogspot.com.br/2013/07/logarithmic-depth-buffer-optimizations.html
// http://outerra.blogspot.com.br/2009/08/logarithmic-z-buffer.html
void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    TexCoords = aTexCoords;
    gl_Position = MVP * vec4(aPos, 1.0);

    float FC = 1.0/log(far*C + 1);
    logz = log(gl_Position.w * C + 1) * FC;

    Normal = normalize(mat3(inversedTransposedModel) * aNormal);
    /*
    vec3 T = vec3(normalize(vec4(aTangent, 0)));
    T = normalize(T - dot(T, Normal) * Normal);
    vec3 B = cross(Normal, T);

    TBN = mat3(T, B, Normal);
    */






    vec3 T = vec3(normalize(inversedTransposedModel * vec4(aTangent, 0)));
    vec3 N = Normal;
//    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);
}

""" }


    /// The Deferred Geometry fragment shader source
    public static var deferredGeometryFragmentShaderSource: String { return """
#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;
struct PhongMaterial {
    sampler2D diffuse;
    sampler2D specular;

    float shininess;

    bool hasNormalMap;
    sampler2D normal;

    bool hasDisplacementMap;
    sampler2D displacementMap;

    float emission;
    bool hasEmissionMap;
    sampler2D emissionMap;
};
uniform PhongMaterial material;

uniform float gamma = 2.2;
in float logz;

void main() {
    gl_FragDepth = logz;

    // store the fragment position vector in the first gbuffer texture
    gPosition = vec4(FragPos, 0.0);

    // also store the per-fragment normals into the gbuffer
    if(material.hasNormalMap) {
        vec3 normal = texture(material.normal, TexCoords).xyz * 2.0 - 1.0;
        if(gl_FrontFacing == false) {
            normal = -normal;
        }
        gNormal.rgb = normalize(TBN * normal);
    } else {
        gNormal.rgb = Normal;
    }
    gNormal.a = material.shininess;

    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = pow(texture(material.diffuse, TexCoords).rgb, vec3(gamma));
    if(material.hasEmissionMap) {
        gAlbedoSpec.rgb += material.emission * texture(material.emissionMap, TexCoords).rgb;
    }

    // store specular intensity in gAlbedoSpec's alpha component
    vec4 specular = texture(material.specular, TexCoords);
    gAlbedoSpec.a = specular.r;
}

""" }

}


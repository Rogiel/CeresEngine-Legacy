
//
//  PBRIrradianceConvolutionShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The PBR Irradiance Convolution vertex shader source
    public static var pbrIrradianceConvolutionVertexShaderSource: String { return """
#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;

void main() {
    WorldPos = aPos;
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}

""" }


    /// The PBR Irradiance Convolution fragment shader source
    public static var pbrIrradianceConvolutionFragmentShaderSource: String { return """
#version 330 core

float DistributionGGX(float NdotH, float roughness) {
    const float PI = 3.14159265359;

    float a2 = pow(roughness, 4);
    float NdotH2 = pow(NdotH, 2.0);

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

// -----------------------------------------------------------------------------

float GeometrySchlickGGX(float NdotV, float roughness) {
    float k = pow(roughness + 1.0, 2) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

// -----------------------------------------------------------------------------

//float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)

float GeometrySmith(float NdotV, float NdotL, float roughness) {
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// -----------------------------------------------------------------------------

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// -----------------------------------------------------------------------------

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

// -----------------------------------------------------------------------------

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
float RadicalInverse_VdC(uint bits) {
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

// -----------------------------------------------------------------------------

vec2 Hammersley(uint i, uint N) {
	return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

// -----------------------------------------------------------------------------

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness) {
    const float PI = 3.14159265359;

	float a = roughness*roughness;

	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

	// from spherical coordinates to cartesian coordinates - halfway vector
	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;

	// from tangent-space H vector to world-space sample vector
	vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}

// -----------------------------------------------------------------------------
struct ImageBasedLightingMaps {
    samplerCube irradianceMap;
    samplerCube prefilterMap;
    sampler2D brdfLUT;
};

vec3 CalcAmbientLighting(ImageBasedLightingMaps maps, vec3 N, vec3 V, vec3 F0, vec3 Albedo, float Metallic, float Roughness, float AmbientOcclusion) {
    vec3 R = reflect(-V, N);
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;

    vec3 irradiance = texture(maps.irradianceMap, N).rgb;
    vec3 diffuse = irradiance * Albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(maps.prefilterMap, R, Roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(maps.brdfLUT, vec2(max(dot(N, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    return (kD * diffuse + specular) * AmbientOcclusion;
}

vec3 IrradianceConvolution(vec3 FragPos, samplerCube environmentMap) {
    const float PI = 3.14159265359;

    vec3 N = normalize(FragPos);

    vec3 irradiance = vec3(0.0);

    // tangent space calculation from origin point
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, N);
    up = cross(N, right);

    float sampleDelta = 0.025;
    float nrSamples = 0.0f;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += pow(texture(environmentMap, sampleVec).rgb, vec3(2.2)) * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    return PI * irradiance * (1.0 / float(nrSamples));
}
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main() {
    FragColor = vec4(
        IrradianceConvolution(WorldPos, environmentMap),
        1.0
    );
}

""" }

}


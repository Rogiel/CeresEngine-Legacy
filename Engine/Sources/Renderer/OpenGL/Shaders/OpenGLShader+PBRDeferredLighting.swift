
//
//  PBRDeferredLightingShader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach.
//

public extension OpenGLShaderProgram {
    
    /// The PBR Deferred Lighting vertex shader source
    public static var pbrDeferredLightingVertexShaderSource: String { return """
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

""" }


    /// The PBR Deferred Lighting fragment shader source
    public static var pbrDeferredLightingFragmentShaderSource: String { return """
#version 330 core

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
struct Camera {
    vec3 position;
    float near;
    float far;
};
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
vec3 PBRPointLightReflectance(vec3 albedo, float metallic, float roughness, PointLight light, vec3 worldPos, vec3 N, vec3 V, vec3 F0) {
    // calculate per-light radiance
    vec3 L = normalize(light.position - worldPos);
    vec3 H = normalize(V + L);
    float distance = length(light.position - worldPos);
    vec3 radiance = light.diffuse / vec3(pow(distance, 2));

    float NdotV = dot(N, V);
    float NdotVm = max(NdotV, 0.0);

    float NdotL = dot(N, L);
    float NdotLm = max(NdotL, 0.0);

    float HdotV = dot(H, V);
    float HdotVm = max(HdotV, 0.0);

    float NdotH = dot(N, H);
    float NdotHm = max(NdotH, 0.0);

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(NdotHm, roughness);
    float G = GeometrySmith(NdotVm, NdotLm, roughness);
    vec3 F = fresnelSchlick(HdotVm, F0);

    vec3 nominator = NDF * G * F;
    float denominator = 4 * NdotVm * NdotLm + 0.00000001; // 0.001 to prevent divide by zero.
    vec3 specular = nominator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // add to outgoing radiance Lo
    // note that we already multiplied the BRDF by the Fresnel (kS) so we won't
    // multiply by kS again
    const float PI = 3.14159265359;
    return (kD * albedo / PI + specular) * radiance * NdotLm;
}
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
in vec2 TexCoords;

uniform sampler2D gPositionAndMetallic;
uniform sampler2D gNormalAndRoughness;
uniform sampler2D gAlbedoTextureAndAmbientOcclusion;

uniform bool hasShadowMap;
uniform samplerCube shadowMap;

uniform PointLight light;
uniform Camera camera;
uniform ImageBasedLightingMaps ibl;

void main() {
    /* G-Buffer */
    vec3 FragPos = texture(gPositionAndMetallic, TexCoords).xyz;
    float Metallic = texture(gPositionAndMetallic, TexCoords).w;
    vec3 Normal = texture(gNormalAndRoughness, TexCoords).xyz;
    float Roughness = texture(gNormalAndRoughness, TexCoords).w;
    vec3 Albedo = texture(gAlbedoTextureAndAmbientOcclusion, TexCoords).xyz;
    float AmbientOcclusion = texture(gAlbedoTextureAndAmbientOcclusion, TexCoords).w;

    /* Standard Parameters */
    vec3 F0 = mix(vec3(0.04), Albedo, Metallic);
    vec3 V = normalize(camera.position - FragPos);

    /* PBR Calculations */
    vec3 Lo = PBRPointLightReflectance(
        /* albedo:   */ Albedo,
        /* metallic: */ Metallic,
        /* roughness:*/ Roughness,
        /* light:    */ light,
        /* worldPos: */ FragPos,
        /* normal:   */ Normal,
        /* view:     */ V,
        /* F0:       */ F0
    );

//                                     IBL  N       V  F0  Albedo  Metallic  Roughness  AmbientOcclusion
    vec3 ambient = CalcAmbientLighting(ibl, Normal, V, F0, Albedo, Metallic, Roughness, AmbientOcclusion);
    vec3 color = ambient + Lo;
    FragColor = vec4(color , 1.0);


//    // ambient lighting (note that the next IBL tutorial will replace
//    // this ambient lighting with environment lighting).
////    vec3 ambient = vec3(0.03) * Albedo * AmbientOcclusion;
////    vec3 color = ambient + Lo;
//
//    // ambient lighting (we now use IBL as the ambient term)
//    vec3 N = Normal;
//    vec3 R = reflect(-V, N);
//    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);
//
//    vec3 kS = F;
//    vec3 kD = 1.0 - kS;
//    kD *= 1.0 - Metallic;
//
//    vec3 irradiance = texture(irradianceMap, N).rgb;
//    vec3 diffuse      = irradiance * Albedo;
//
//    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
//    const float MAX_REFLECTION_LOD = 4.0;
//    vec3 prefilteredColor = textureLod(prefilterMap, R,  Roughness * MAX_REFLECTION_LOD).rgb;
//    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), Roughness)).rg;
//    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
//
//    vec3 ambient = (kD * diffuse + specular) * AmbientOcclusion;
//    vec3 color = ambient + Lo;
//
//    FragColor = vec4(color , 1.0);
}

""" }

}


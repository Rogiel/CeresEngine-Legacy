#pragma once

#include "PBRModel.glsl"

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
    vec3 diffuse      = irradiance * Albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(maps.prefilterMap, R,  Roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(maps.brdfLUT, vec2(max(dot(N, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    return (kD * diffuse + specular) * AmbientOcclusion;
}

vec3 IrradianceConvolution(vec3 FragPos, samplerCube environmentMap) {
    const float PI = 3.14159265359;

    vec3 N = normalize(FragPos);

    vec3 irradiance = vec3(0.0);

    // tangent space calculation from origin point
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, N);
    up         = cross(N, right);

    float sampleDelta = 0.025;
    float nrSamples = 0.0f;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += pow(texture(environmentMap, sampleVec).rgb, vec3(2.2)) * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    return PI * irradiance * (1.0 / float(nrSamples));
}
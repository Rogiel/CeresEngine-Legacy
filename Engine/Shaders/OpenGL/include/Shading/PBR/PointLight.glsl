#pragma once

#include "PBRModel.glsl"
#include "Object/PointLight.glsl"

vec3 PBRPointLightReflectance(vec3 albedo, float metallic, float roughness, PointLight light, vec3 worldPos, vec3 N, vec3 V, vec3 F0) {
    // calculate per-light radiance
    vec3 L = normalize(light.position - worldPos);
    vec3 H = normalize(V + L);
    float distance = length(light.position - worldPos);
    vec3 radiance = light.diffuse / vec3(pow(distance, 2));

    float NdotV = dot(N, V);
    float NdotVm = max(NdotV, 0.0);

    float NdotL  = dot(N, L);
    float NdotLm = max(NdotL, 0.0);

    float HdotV  = dot(H, V);
    float HdotVm = max(HdotV, 0.0);

    float NdotH  = dot(N, H);
    float NdotHm = max(NdotH, 0.0);

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(NdotHm, roughness);
    float G   = GeometrySmith(NdotVm, NdotLm, roughness);
    vec3 F    = fresnelSchlick(HdotVm, F0);

    vec3 nominator    = NDF * G * F;
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

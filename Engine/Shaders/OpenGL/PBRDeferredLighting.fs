#version 330 core

#include "Object/PointLight.glsl"
#include "Object/Camera.glsl"
#include "Shading/PBR.glsl"
#include "Shading/PBR/ImageBasedLighting.glsl"

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
    vec3  FragPos            = texture(gPositionAndMetallic, TexCoords).xyz;
    float Metallic           = texture(gPositionAndMetallic, TexCoords).w;
    vec3  Normal             = texture(gNormalAndRoughness, TexCoords).xyz;
    float Roughness          = texture(gNormalAndRoughness, TexCoords).w;
    vec3  Albedo             = texture(gAlbedoTextureAndAmbientOcclusion, TexCoords).xyz;
    float AmbientOcclusion   = texture(gAlbedoTextureAndAmbientOcclusion, TexCoords).w;

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
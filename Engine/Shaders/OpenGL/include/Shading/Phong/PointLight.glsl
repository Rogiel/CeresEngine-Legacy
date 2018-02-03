#pragma once

#include "Object/PointLight.glsl"

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
        light.diffuse  * diff * attenuation * visibility,
        light.specular * spec * attenuation * visibility
    );
}

vec3 sampleOffsetDirections[20] = vec3[](
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
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

#pragma once

#include "Object/DirectionalLight.glsl"

// calculates the color when using a directional light.
mat3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, float shininess) {
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    // combine results
    return mat3(
        light.ambient,
        light.diffuse * diff,
        light.specular * spec
    );
}

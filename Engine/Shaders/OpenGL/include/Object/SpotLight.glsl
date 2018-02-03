#pragma once

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

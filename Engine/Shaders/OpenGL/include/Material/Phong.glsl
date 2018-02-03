#pragma once

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
#pragma once

struct PBRMaterial {
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D ambientOcclusionMap;

    sampler2D displacementMap;
    float maximumDisplacement;

    sampler2D emissionMap;
};

//
// Created by Rogiel Sulzbach on 7/23/17.
//

#include "OpenGLDeferredRendering.hpp"

namespace XYZ::Graphics::Renderer::OpenGL {

	const Shader::ShaderSource GeometryVertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBinormal;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out mat3 TBN;

uniform mat4 model;
uniform mat4 inversedTransposedModel;
layout(std140) uniform ViewProjection {
    mat4 projection;
    mat4 view;
    struct {
        vec3 position;
    } camera;
};

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    TexCoords = aTexCoords;
    Normal = mat3(inversedTransposedModel) * aNormal;
    gl_Position = projection * view * worldPos;

    vec3 T = vec3(normalize(inversedTransposedModel * vec4(aTangent, 0)));
    vec3 N = vec3(normalize(inversedTransposedModel * vec4(aNormal, 0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);
}
)";

	const Shader::ShaderSource GeometryFragmentShaderSource = R"(
#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

struct PhongMaterial {
    vec3 diffuseColor;
    sampler2D diffuse;

    vec3 specularColor;
    sampler2D specular;

    float shininess;

    bool hasNormalMap;
    sampler2D normal;

    bool hasDisplacementMap;
    sampler2D displacementMap;

    bool blinn;
};
uniform PhongMaterial material;

void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition.xyz = FragPos;
    gPosition.w = 0.0;

    // also store the per-fragment normals into the gbuffer
    vec3 bump = texture(material.normal, TexCoords).xyz * 2.0 - 1.0;
    gNormal.rgb = normalize(TBN * bump);
    gNormal.a = material.shininess;

    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(material.diffuse, TexCoords).rgb + material.diffuseColor;

    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(material.specular, TexCoords).r + material.specularColor.r;
}
)";

	// -----------------------------------------------------------------------------------------------------------------

	const Shader::ShaderSource LightingVertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}
)";

	const Shader::ShaderSource DirectionalLightFragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

layout(std140) uniform ViewProjection {
    mat4 projection;
    mat4 view;
    struct {
        vec3 position;
    } camera;
};

uniform struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

// function prototypes
mat3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, float shininess);

void main() {
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    float Depth = texture(gPosition, TexCoords).a;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    float Shininess = texture(gNormal, TexCoords).a;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 viewDir = normalize(camera.position - FragPos);
    mat3 result = CalcDirectionalLight(light, Normal, viewDir, Shininess);

    vec3 materialDiffuse = Diffuse;
    vec3 materialSpecular = vec3(Specular);
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;

    FragColor = vec4(color, 1.0);
}

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
)";

	const Shader::ShaderSource PointLightFragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

layout(std140) uniform ViewProjection {
    mat4 projection;
    mat4 view;
    struct {
        vec3 position;
    } camera;
};

uniform struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light;

// function prototypes
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess);

void main() {
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    float Depth = texture(gPosition, TexCoords).a;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    float Shininess = texture(gNormal, TexCoords).a;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 viewDir = normalize(camera.position - FragPos);
    mat3 result = CalcPointLight(light, Normal, FragPos, viewDir, Shininess);

    vec3 materialDiffuse = Diffuse;
    vec3 materialSpecular = vec3(Specular);
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;

    FragColor = vec4(color, 1.0);
}

// calculates the color when using a point light.
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess) {
    vec3 lightPos = light.position;
    vec3 lightDir = normalize(lightPos - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    // attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));

    // combine results
    return mat3(
        light.ambient * attenuation,
        light.diffuse * diff * attenuation,
        light.specular * spec * attenuation
    );
}
)";

	const Shader::ShaderSource SpotLightFragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D shadowMap;

uniform mat4 lightSpaceMatrix;

layout(std140) uniform ViewProjection {
    mat4 projection;
    mat4 view;
    struct {
        vec3 position;
    } camera;
};

uniform struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

// function prototypes
mat3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main() {
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    float Depth = texture(gPosition, TexCoords).a;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    float Shininess = texture(gNormal, TexCoords).a;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 viewDir = normalize(camera.position - FragPos);
    mat3 result = CalcSpotLight(light, Normal, FragPos, viewDir, Shininess);

    vec3 materialDiffuse = Diffuse;
    vec3 materialSpecular = vec3(Specular);
    vec3 color = (result[0] + result[1]) * materialDiffuse + result[2] * materialSpecular;

    FragColor = vec4(color, 1.0);
}

// calculates the color when using a spot light.
mat3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shininess) {
	vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// shadows
	float visibility = ShadowCalculation(lightSpaceMatrix * vec4(fragPos, 1.0), normal, lightDir);

    // combine results
    return mat3(
        light.ambient * attenuation * intensity,
        light.diffuse * diff * attenuation * intensity * visibility,
        light.specular * spec * attenuation * intensity * visibility
    );
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// if outside of shadow map bounds, return no shadows
	if(projCoords.z > 1.0)
        return 1.0;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

	// compute the bias factor based on the fragment distance from the light
//	float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
	const float bias = 0.0;

	// apply a PCSS technique to get a bit better shadows
	const int smoothing = 2;
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -smoothing; x <= smoothing; ++x) {
		for(int y = -smoothing; y <= smoothing; ++y) {
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 0.5 : 1.0;
		}
	}
	shadow /= pow(2.0 * float(smoothing) + 1.0, 2);

	// return the shadow to the caller
	return shadow;
}
)";

	const Shader::ShaderSource ShadowMapVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
)";

	const Shader::ShaderSource ShadowMapFragmentShaderSource = R"(
#version 330 core
void main() {}
)";

}
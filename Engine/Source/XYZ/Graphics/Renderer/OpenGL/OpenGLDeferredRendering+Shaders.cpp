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
	vec3 normal = texture(material.normal, TexCoords).xyz * 2.0 - 1.0;
	if(gl_FrontFacing == false) {
    	normal = -normal;
	}
    gNormal.rgb = normalize(TBN * normal);
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
uniform samplerCube shadowMap;
uniform float farPlane;

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
float ShadowCalculation(vec3 fragPos, vec3 lightPos, vec3 viewPos);

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

//	float shadow = ShadowCalculation(FragPos, light.position);
//	FragColor = vec4(shadow, shadow, shadow, 1.0);
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

//	// compute the shadow visibility factor
	float visibility = ShadowCalculation(fragPos, lightPos, camera.position);
//	const float visibility = 1.0;

    // combine results
    return mat3(
        light.ambient * attenuation,
        light.diffuse * diff * attenuation * visibility,
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

float ShadowCalculation(vec3 fragPos, vec3 lightPos, vec3 viewPos) {
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
    float shadow = currentDepth - bias > closestDepth ? 0.5 : 1.0;

//	float shadow = 0.0;
//	const float bias   = 0.15;
//	const int samples  = 2;
//	float viewDistance = length(viewPos - fragPos);
//	const float diskRadius = 0.05;
//	int i = 0;
////	for(int i = 0; i < 2; i++) {
//		float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r * farPlane;
//		shadow += currentDepth - bias > closestDepth ? 0.0 : 1.0;
////	}
//	shadow /= float(samples);

	return shadow;
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

	const Shader::ShaderSource DirectionalLightShadowMapVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
)";

	const Shader::ShaderSource DirectionalLightShadowMapFragmentShaderSource = R"(
#version 330 core
void main() {}
)";

	const Shader::ShaderSource PointLightShadowMapVertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 FragPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
	FragPos = model * vec4(aPos, 1.0);
    gl_Position = lightSpaceMatrix * FragPos;
}
)";

//	const Shader::ShaderSource PointLightShadowMapGeometryShaderSource = R"(
//#version 330 core
//layout (triangles) in;
//layout (triangle_strip, max_vertices=18) out;
//
//uniform mat4 lightSpaceMatrices[6];
//
//out vec4 FragPos; // FragPos from GS (output per emitvertex)
//
//void main() {
//    for(int face = 0; face < 6; ++face) {
//        gl_Layer = face; // built-in variable that specifies to which face we render.
//        for(int i = 0; i < 3; ++i) // for each triangle's vertices
//        {
//            FragPos = gl_in[i].gl_Position;
//            gl_Position = lightSpaceMatrices[face] * FragPos;
//            EmitVertex();
//        }
//        EndPrimitive();
//    }
//}
//)";

	const Shader::ShaderSource PointLightShadowMapFragmentShaderSource = R"(
#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() {
    float lightDistance = length(FragPos.xyz - lightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / farPlane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}
)";

	const Shader::ShaderSource SpotLightShadowMapVertexShaderSource = DirectionalLightShadowMapVertexShaderSource;
	const Shader::ShaderSource SpotLightShadowMapFragmentShaderSource = DirectionalLightShadowMapFragmentShaderSource;

	// -----------------------------------------------------------------------------------------------------------------

	const Shader::ShaderSource BloomExposureMappingVertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos, 1.0);
	TexCoords = aTexCoords;
}
)";
	const Shader::ShaderSource BloomExposureMappingFragmentShaderSource = R"(
#version 330 core

in vec2 TexCoords;
out vec4 BloomColor;

uniform sampler2D scene;

void main() {
	vec3 fragColor = texture(scene, TexCoords).rgb;

	// check whether fragment output is higher than threshold, if so output as brightness color
	float brightness = dot(fragColor, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0) {
		BloomColor = vec4(vec3(brightness), 1.0);
	} else {
		BloomColor = vec4(0.0);
	}
}
)";

	const Shader::ShaderSource BloomBlurVertexShaderSource = BloomExposureMappingVertexShaderSource;
	const Shader::ShaderSource BloomBlurFragmentShaderSource = R"(
#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D scene;

uniform bool horizontal;

const float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main() {
     vec2 tex_offset = 1.0 / textureSize(scene, 0); // gets size of single texel
     vec3 result = texture(scene, TexCoords).rgb * weight[0];
     if(horizontal) {
         for(int i = 1; i < 5; ++i) {
            result += texture(scene, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(scene, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
     } else {
         for(int i = 1; i < 5; ++i) {
             result += texture(scene, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture(scene, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
     }
     FragColor = vec4(result, 1.0);
}
)";

	// -----------------------------------------------------------------------------------------------------------------

	const Shader::ShaderSource HDRVertexShaderSource = BloomExposureMappingVertexShaderSource;
	const Shader::ShaderSource HDRFragmentShaderSource = R"(
#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform float exposure;

uniform sampler2D scene;
uniform sampler2D bloom;

uniform bool hdr = true;

void main() {
    const float gamma = 2.2;

	// fetch the scene color
	vec3 hdrColor = vec3(0.0);
	hdrColor += texture(scene, TexCoords).rgb;
	hdrColor += texture(bloom, TexCoords).rgb;

	if(hdr) {
		// tone mapping
		vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

		// also gamma correct while we're at it
		result = pow(result, vec3(1.0 / gamma));
		FragColor = vec4(result, 1.0);
	} else {
		FragColor = vec4(hdrColor, 1.0);
	}
}
)";

}
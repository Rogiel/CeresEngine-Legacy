// retrieve data from gbuffer
vec3 FragPos = texture(gPosition, TexCoords).rgb;

float Depth = texture(gPosition, TexCoords).a;
vec3 Normal = texture(gNormal, TexCoords).rgb;
float Shininess = texture(gNormal, TexCoords).a;
vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
float Specular = texture(gAlbedoSpec, TexCoords).a;
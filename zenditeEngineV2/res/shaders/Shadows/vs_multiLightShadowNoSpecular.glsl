#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

#define MAX_DIR_LIGHTS 10
#define MAX_SPOT_LIGHTS 10
#define MAX_POINT_LIGHTS 10

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosDirLightSpace[MAX_DIR_LIGHTS];
    vec4 FragPosSpotLightSpace[MAX_DIR_LIGHTS];
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 dirLightSpaceMatrixes[MAX_DIR_LIGHTS];
uniform mat4 spotLightSpaceMatrixes[MAX_SPOT_LIGHTS];
uniform int nrDirLights;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    for (int i = 0; i < nrDirLights; i++) {
		vs_out.FragPosDirLightSpace[i] = dirLightSpaceMatrixes[i] * vec4(vs_out.FragPos, 1.0);
		vs_out.FragPosSpotLightSpace[i] = spotLightSpaceMatrixes[i] * vec4(vs_out.FragPos, 1.0);
    }

    gl_Position = projection * view * model * vec4(aPos, 1.0);

}

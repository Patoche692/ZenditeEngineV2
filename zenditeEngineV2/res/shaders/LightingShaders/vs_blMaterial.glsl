#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 fragPos;
out vec3 Normal;
out vec2 texCoords;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    mat4 MVP = projMat * viewMat * modelMat;

    fragPos = vec3(modelMat * vec4(aPos, 1.0));
    Normal = aNormal;
    texCoords = aTexCoords;

    gl_Position = MVP * vec4(aPos, 1.0);

}
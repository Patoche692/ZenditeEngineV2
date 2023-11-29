#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 fragPos;
out vec3 Normal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    mat4 MVP = projMat * viewMat * modelMat;

    fragPos = vec3(modelMat * vec4(aPos, 1.0));
    Normal = aNormal;

    gl_Position = MVP * vec4(aPos, 1.0);

}
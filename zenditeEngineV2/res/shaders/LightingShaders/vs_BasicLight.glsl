#version 420 core
layout(location = 0) in vec3 aPos;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    mat4 MVP = projMat * viewMat * modelMat;

    gl_Position = MVP * vec4(aPos, 1.0);
    
}
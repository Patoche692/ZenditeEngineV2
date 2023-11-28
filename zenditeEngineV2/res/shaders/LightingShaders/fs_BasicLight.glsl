#version 420 core

in vec3 Normal;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientLightStrength = 0.1;
    vec3 ambientLight = ambientLightStrength * lightColor;

    vec3 result = ambientLight * objectColor;

    FragColor = vec4(result, 1.0);
}
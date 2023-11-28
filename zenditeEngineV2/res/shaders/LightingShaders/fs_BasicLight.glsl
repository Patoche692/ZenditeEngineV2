#version 420 core

in vec3 fragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightWorldPos;

void main()
{
    vec3 lightDir = normalize(lightWorldPos - fragPos);
    vec3 norm = normalize(Normal);

    float lightIntensity = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = lightIntensity * lightColor;

    float ambientLightStrength = 0.1;
    vec3 ambientLight = ambientLightStrength * lightColor;

    vec3 result = (ambientLight + diffuseLight)*objectColor;

    FragColor = vec4(result, 1.0);
}
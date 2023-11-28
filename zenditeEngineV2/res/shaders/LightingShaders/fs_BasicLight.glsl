#version 420 core

in vec3 fragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightWorldPos;
uniform vec3 cameraWorldPos;

uniform int specularIntensity;
uniform float specularStrength;

void main()
{
    vec3 lightDir = normalize(lightWorldPos - fragPos);
    vec3 norm = normalize(Normal);

    //diffuse light calc:
    float lightIntensity = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = lightIntensity * lightColor;

    //specualr light calc:
    vec3 reflection = normalize(2 * (dot(norm, lightDir)) * norm - lightDir);
    vec3 viewDir = normalize(cameraWorldPos - fragPos);
    float specVal = pow(max(dot(reflection, viewDir), 0.0), specularIntensity);
    vec3 specular = specularStrength * specVal * lightColor;

    //ambient light calc:
    float ambientLightStrength = 0.1;
    vec3 ambientLight = ambientLightStrength * lightColor;

    vec3 result = (ambientLight + diffuseLight + specular)*objectColor;

    FragColor = vec4(result, 1.0);
}
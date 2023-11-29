#version 420 core

struct Material
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess; //AKA: specularStrength
};

struct Light {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragPos;
in vec3 Normal;

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform vec3 lightColor;

uniform vec3 lightWorldPos;
uniform vec3 cameraWorldPos;

void main()
{
    vec3 lightDir = normalize(lightWorldPos - fragPos);
    vec3 norm = normalize(Normal);

    //ambient light:
    vec3 ambientLight = light.ambient * material.ambientColor;

    //diffuse Light:
    float lightIntensity = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = (lightIntensity * material.diffuseColor) * light.diffuse;

    //Specular Light:
    vec3 reflection = normalize(2 * (dot(norm, lightDir)) * norm - lightDir);
    vec3 viewDir = normalize(cameraWorldPos - fragPos);
    float shineVal = pow(max(dot(reflection, viewDir), 0.0), material.shininess);
    vec3 specularLight = light.specular * (shineVal * material.specularColor);

    vec3 result = ambientLight + diffuseLight + specularLight;

    FragColor = vec4(result, 1.0);
}
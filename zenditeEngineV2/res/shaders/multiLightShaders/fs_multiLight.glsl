#version 420 core
#define NR_POINT_LIGHTS 4

//Instead of setting the material color we need to sample from a texture.
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess; //AKA: specularStrength
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 fragPos;
in vec3 Normal;
in vec2 texCoords;

out vec4 FragColor;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];

uniform vec3 cameraWorldPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraWorldPos - fragPos);

    vec3 totalDirLight = CalcDirLight(dirLight, norm, viewDir);
    vec3 totalPointLight = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        totalPointLight = totalPointLight + CalcPointLight(pointLight[i], norm, fragPos, viewDir);
    }
    

    vec3 result = totalPointLight;// +totalDirLight;

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirLight.direction); //we want the negative as the lighting calculations want the vector to point towards no from the light source

    //Ambient
    vec3 ambient = dirLight.ambient * texture(material.diffuse, texCoords).rgb;

    //Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * dirLight.diffuse * texture(material.diffuse, texCoords).rgb;

    //Specular
    vec3 reflection = normalize(2 * (dot(normal, lightDir)) * normal - lightDir);
    float spec = pow(max(dot(reflection, viewDir), 0.0), material.shininess);
    vec3 specular = dirLight.specular * spec * texture(material.specular, texCoords).rgb;

    return (specular + diffuse + ambient);
}


vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float distance = length(pointLight.position - fragPos);
    float lightAttenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    //ambient light:
    vec3 ambient = pointLight.ambient * texture(material.diffuse, texCoords).rgb;
    ambient = ambient * lightAttenuation;

    //diffuse Light:
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * pointLight.diffuse * texture(material.diffuse, texCoords).rgb;
    diffuse = diffuse * lightAttenuation;

    //Specular Light:
    vec3 reflection = normalize(2 * (dot(normal, lightDir)) * normal - lightDir);
    float spec = pow(max(dot(reflection, viewDir), 0.0), material.shininess);
    vec3 specular = pointLight.specular * spec * texture(material.specular, texCoords).rgb;
    specular = specular * lightAttenuation;

    return (ambient + diffuse + specular);
}
/*
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTexCoord;

out vec2 texCoord;
out float Height;
//out float Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Height = aPos.y;
    //Position = (view * model * vec4(aPos, 1.0)).xyz;
    texCoord = aTexCoord;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
*/

#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 fragPos;
out vec3 Normal;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 MVP = projection * view * model;

    fragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
    texCoords = aTexCoords;

    gl_Position = MVP * vec4(aPos, 1.0);

}
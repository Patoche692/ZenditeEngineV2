#version 420 core

in vec2 texCoord;
out vec4 FragColor;

//uniform vec4 ourColor;

uniform sampler2D colorTexture1;
uniform sampler2D colorTexture2;

void main()
{
    FragColor = mix(texture(colorTexture1, texCoord), texture(colorTexture2, vec2(1.0 - texCoord.x, texCoord.y)), 0.2);

}
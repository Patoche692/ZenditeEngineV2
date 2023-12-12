#version 420 core

in vec2 texCoord;
out vec4 FragColor;

//uniform vec4 ourColor;

uniform sampler2D colorTexture;

void main()
{

    FragColor = texture(colorTexture, texCoord);

    
}
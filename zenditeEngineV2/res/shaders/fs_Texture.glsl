#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform vec4 ourColor;

//The below is a method of getting the texture from textue unit 0 without manually having to send it in yourself.
uniform sampler2D colorTexture;

void main()
{
    FragColor = texture(colorTexture, texCoord);
}
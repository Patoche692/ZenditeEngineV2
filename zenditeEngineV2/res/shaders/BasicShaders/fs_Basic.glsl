#version 330 core
out vec4 FragColor;

uniform vec4 ourColor;

//The below is a method of getting the texture from textue unit 0 without manually having to send it in yourself.
//layout(binding = 0) uniform sampler2D colortexture;

void main()
{
    FragColor = ourColor;
}
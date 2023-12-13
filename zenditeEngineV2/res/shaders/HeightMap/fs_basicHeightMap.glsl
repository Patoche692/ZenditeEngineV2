#version 420 core

in vec2 texCoord;
in float Height;
//in vec3 Position;

out vec4 FragColor;

//uniform vec4 ourColor;

//uniform sampler2D colorTexture;

void main()
{
    float h = (Height + 16) / 32.0f;

    FragColor = vec4(h, h, h, 1.0);
    
    //FragColor = texture(colorTexture, texCoord);

}
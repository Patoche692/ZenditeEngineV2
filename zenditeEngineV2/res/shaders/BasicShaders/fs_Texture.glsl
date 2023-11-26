#version 420 core

in vec2 texCoord;
out vec4 FragColor;

//uniform vec4 ourColor;

//The below is a method of getting the texture from textue unit 0 without manually having to send it in yourself.
uniform sampler2D colorTexture1;
uniform sampler2D colorTexture2;

void main()
{

    //vec4 color1 = texture(colorTexture1, texCoord); // Color from first texture
    //vec4 color2 = texture(colorTexture2, texCoord); // Color from second texture

    //vec3 blendedColor = mix(color1, color2, 0.2); //texture performs a color lookup at the coordinates texCoords
    
    FragColor = mix(texture(colorTexture1, texCoord), texture(colorTexture2, vec2(1.0-texCoord.x, texCoord.y)), 0.2);

    //FragColor = texture(colorTexture1, texCoord);
}
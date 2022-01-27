#version 410 core

in vec3 TextureCoordinates;
uniform samplerCube CubeMap;

out vec4 FragColor;

void main()
{
    FragColor = texture(CubeMap, TextureCoordinates);
}

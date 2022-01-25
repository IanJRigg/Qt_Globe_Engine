#version 410 core

in vec3 texture_coordinates;
uniform samplerCube cubemap;

out vec4 FragColor;

void main()
{
    FragColor = texture(cubemap, texture_coordinates);
}

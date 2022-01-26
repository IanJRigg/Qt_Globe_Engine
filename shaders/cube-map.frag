#version 410 core

// in vec3 texture_coordinates;
// uniform samplerCube cubemap;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); // texture(cubemap, texture_coordinates);
}

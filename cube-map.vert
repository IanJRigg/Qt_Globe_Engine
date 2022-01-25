#version 410 core

layout (location = 0) in vec3 aPos; // the position has attribute position 0

out vec3 texture_coordinates;

uniform mat4 mvp;

void main()
{
    texture_coordinates = aPos;
    gl_Position = mvp * vec4(aPos, 1.0);
}

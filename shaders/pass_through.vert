#version 330 core 
// vertex shader

// input: attribute named 'position' with 3 floats per vertex
layout (location = 0) in vec3 position;

out vec4 fragColor;

void main() {
  gl_Position = vec4(position, 1.0);
  fragColor = vec4(vec3(1.0, 1.0f, 1.0f), 1.0); // Make everything full white for visibility
}


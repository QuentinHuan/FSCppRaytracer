#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;

uniform vec3 lColor;

void main() { FragColor = vec4(lColor, 1.0f); };
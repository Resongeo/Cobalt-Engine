#version 450 core

layout(location = 0) out vec4 FragColor;

// -- Vertex data from the vertex shader --
in vec4 v_Color;

void main() {
    FragColor = v_Color;
}
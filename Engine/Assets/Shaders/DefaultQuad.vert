#version 450 core

// -- Attribute data --
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;

// TODO: Change to uniform buffers
uniform mat4 u_ViewProjection;

// -- Vertex data for the fragment shader --
out vec4 v_Color;

void main() {
    gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
    v_Color = a_Color;
}

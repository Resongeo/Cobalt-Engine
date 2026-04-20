#version 450 core

// -- Attribute data --
layout(location = 0) in vec2  a_Position;
layout(location = 1) in vec2  a_TexCoords;
layout(location = 2) in vec4  a_Color;
layout(location = 3) in float a_TexIndex;

// TODO: Change to uniform buffers
uniform mat4 u_ViewProjection;

// -- Vertex data for the fragment shader --
out vec2  v_TexCoords;
out vec4  v_Color;
out float v_TexIndex;

void main() {
    gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
    v_TexCoords = a_TexCoords;
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
}

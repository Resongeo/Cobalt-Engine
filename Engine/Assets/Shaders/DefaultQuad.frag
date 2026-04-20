#version 450 core

// -- Output data --
layout(location = 0) out vec4 FragColor;

// TODO: Change to uniform buffers
uniform sampler2D u_TextureSlots[32];

// -- Vertex data from the vertex shader --
in vec2  v_TexCoords;
in vec4  v_Color;
in float v_TexIndex;

void main() {
    if (v_TexIndex == 0.0) {
        FragColor = v_Color;
    } else {
        FragColor = texture(u_TextureSlots[int(v_TexIndex)], v_TexCoords) * v_Color;
    }

    if (FragColor.a == 0.0) {
        discard;
    }
}
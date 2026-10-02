struct SpriteData
{
    float3 position;
    float rotation;
    float2 scale;
    float2 padding;
    float tex_u, tex_v, tex_w, tex_h;
    float4 color;
};

StructuredBuffer<SpriteData> DataBuffer : register(t0, space0);

cbuffer UniformBlock : register(b0, space1) {
    float4x4 view_projection_matrix;
};

struct Output
{
    float2 texcoord : TEXCOORD0;
    float4 color : TEXCOORD1;
    float4 position : SV_Position;
};

static const uint triangle_indices[6] = {0, 1, 2, 3, 2, 1};
static const float2 vertex_pos[4] = {{-0.5f, -0.5f}, {0.5f, -0.5f}, {-0.5f, 0.5f}, {0.5f, 0.5f}};

Output main(uint id : SV_VertexID) {
    uint sprite_index = id / 6;
    uint vert = triangle_indices[id % 6];
    SpriteData sprite = DataBuffer[sprite_index];

    float2 texcoord[4] = {{sprite.tex_u, sprite.tex_v},
                          {sprite.tex_u + sprite.tex_w, sprite.tex_v},
                          {sprite.tex_u, sprite.tex_v + sprite.tex_h},
                          {sprite.tex_u + sprite.tex_w, sprite.tex_v + sprite.tex_h}};

    float c = cos(sprite.rotation);
    float s = sin(sprite.rotation);

    float2 coord = vertex_pos[vert] * sprite.scale;
    float2x2 rotation = {c, s, -s, c};
    coord = mul(coord, rotation);

    float3 world_pos = float3(coord + sprite.position.xy, sprite.position.z);

    Output output;
    output.position = mul(view_projection_matrix, float4(world_pos, 1.0f));
    output.texcoord = texcoord[vert];
    output.color = sprite.color;

    return output;
}

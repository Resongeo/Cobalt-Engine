Texture2D<float4> SpriteTexture : register(t0, space2);
SamplerState SpriteSampler : register(s0, space2);

struct Input
{
    float2 texcoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

float4 main(Input input) : SV_Target0 {
    return input.color * SpriteTexture.Sample(SpriteSampler, input.texcoord);
}

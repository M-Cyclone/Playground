// For fragment shader, register spaces are:
// Sampler: space2
// Texture: space2
// RWTex: space2
// RWBuf: space2
// Uniform: space3

struct Output
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
};

Texture2D<float4> sc_texture : register(t0, space2);
SamplerState sc_sampler : register(s0, space2);

float4 main(Output vs_input) : SV_Target0
{
    return sc_texture.Sample(sc_sampler, vs_input.texcoord);
}

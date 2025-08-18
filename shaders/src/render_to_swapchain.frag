struct Output
{
    float2 texcoord : TEXCOORD0;
    float4 position : SV_Position;
};

Texture2D<float4> sc_texture : register(t0, space2);
SamplerState sc_sampler : register(s0, space2);

float4 main(Output vs_input) : SV_Target0
{
    return sc_texture.Sample(sc_sampler, vs_input.texcoord);
}

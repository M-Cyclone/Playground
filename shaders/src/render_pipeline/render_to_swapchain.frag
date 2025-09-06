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

float4 main(Output vs_input) : SV_Target0
{
    return float4(vs_input.texcoord, 1.0f, 1.0f);
}

// For vertex shader, register spaces are:
// Sampler: space0
// Texture: space0
// RWTex: space0
// RWBuf: space0
// Uniform: space1

struct Input
{
    float2 position : TEXCOORD0;
    float2 texcoord : TEXCOORD1;
};

struct Output
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
};

Output main(Input input)
{
    Output output;
    output.position = float4(input.position, 0.0f, 1.0f);
    output.texcoord = float2(input.texcoord.x, 1.0f - input.texcoord.y);
    return output;
}

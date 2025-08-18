struct Input
{
    float2 position : TEXCOORD0;
    float2 texcoord : TEXCOORD1;
};

struct Output
{
    float2 texcoord : TEXCOORD0;
    float4 position : SV_Position;
};

Output main(Input input)
{
    Output output;
    output.texcoord = float2(input.texcoord.x, -input.texcoord.y);
    output.position = float4(input.position, 0.0f, 1.0f);
    return output;
}

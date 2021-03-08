Texture2D tex;

SamplerState splr;

float4 main(float2 pos : POSITION) : SV_TARGET
{
    return tex.Sample(splr, float2((pos.x + 1) / 2, (-pos.y + 1) / 2));
}
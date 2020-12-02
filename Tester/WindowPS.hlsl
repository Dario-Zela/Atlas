Texture2D tex;

SamplerState splr;

float4 main(float2 pos : POSITION) : SV_TARGET
{
    return tex.Sample(splr, (pos + 1) / 2);
}
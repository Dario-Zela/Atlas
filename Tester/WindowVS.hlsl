struct SV_OUT
{
    float2 pos : POSITION;
    float4 pos2 : SV_POSITION;
};

SV_OUT main(float3 pos : POSITION)
{
    SV_OUT out3 = { float2(pos.x, pos.y), float4(pos, 1) };
    return out3;
}
#pragma once
struct SV_OUT
{
	float3 pos : POSITION;
    float2 tex : TEXCOORD;
	float4 pos2 : SV_POSITION;
};

cbuffer SV_IN
{
	matrix transform;
};

cbuffer SV_IN2
{
    matrix model;
};

SV_OUT main(float3 pos : POSITION, float2 textu : TEXCOORD)
{
    SV_OUT vso = { (float3) mul(float4(pos, 1.0f), model), textu, mul(float4(pos, 1.0f), transform) };
	
	return vso;
}
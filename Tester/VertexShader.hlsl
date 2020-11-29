#pragma once
struct SV_OUT
{
	float4 Color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer SV_IN
{
	matrix transform;
};

SV_OUT main(float3 pos : POSITION)
{
	SV_OUT vso = { float4(pos, 1.0f),  mul(float4(pos, 1.0f) , transform) };
	return vso;
}
struct SV_OUT
{
	float4 Color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer SV_IN
{
	matrix transform;
};

SV_OUT main( float2 pos : aPosition, float3 color : aColor)
{
	SV_OUT vso = { float4(color, 1.0f), mul(float4(pos, 0.0f, 1.0f), transform) };
	return vso;
}
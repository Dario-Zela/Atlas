struct SV_OUT
{
	float4 Color : COLOR;
	float4 pos : SV_POSITION;
};

SV_OUT main( float2 pos : POSITION, float3 color : COLOR )
{
	SV_OUT vso = { float4(color, 1.0f),float4(pos, 0.0f, 1.0f) };
	return vso;
}
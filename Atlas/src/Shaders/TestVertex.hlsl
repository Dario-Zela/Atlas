/*
struct SV_OUT
{
	float2 TexCoords : TEXTURECOORDS;
	float4 pos : SV_POSITION;
};

cbuffer SV_IN
{
	matrix transform; 
};

SV_OUT main( float3 pos : POSITION, float2 TexCoords : TEXTURECOORDS)
{
	SV_OUT vso = { TexCoords,  mul(float4(pos, 1.0f) , transform) };
	return vso;
}
*/
///*
struct SV_OUT
{
	float4 Color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer SV_IN
{
	matrix transform;
};

SV_OUT main(float3 pos : POSITION, float4 Color : COLOR)
{
	SV_OUT vso = { Color,  mul(float4(pos, 1.0f) , transform) };
	return vso;
}
//*/
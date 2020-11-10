///*
struct SV_OUT
{
	float2 TexCoords : TEXTURECOORDS;
	float4 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer SV_IN
{
	matrix transform; 
};

cbuffer SV_IN2
{
	float3 color;
	float shad;
};

SV_OUT main(float3 pos : POSITION, float2 TexCoords : TEXTURE_COORDS)
{
	SV_OUT vso = { TexCoords, float4(color, 1.0f) ,  mul(float4(pos, 1.0f) , transform) };
	SV_OUT vso2 = { TexCoords, float4(1.0f,1.0f,1.0f,1.0f) ,  mul(float4(pos, 1.0f) , transform) };
	if (shad == 0)
	{
		return vso;
	}
	else
	{
		return vso2;
	}
	
}
//*/
/*
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
//*/
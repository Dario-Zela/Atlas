///*
Texture2D tex;

SamplerState splr;

float4 main(float2 TexCoords : TEXTURECOORDS, float4 color : COLOR) : SV_TARGET
{
	return (tex.Sample(splr, TexCoords) + color) / 2;
}
//*/

/*
float4 main(float4 Color : COLOR) : SV_TARGET
{
	return Color;
}
//*/
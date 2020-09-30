///*
Texture2D tex;

SamplerState splr;

float4 main(float2 TexCoords : TEXTURECOORDS) : SV_TARGET
{
	return tex.Sample(splr, TexCoords);
}
//*/

/*
float4 main(float4 Color : COLOR) : SV_TARGET
{
	return Color;
}
*/
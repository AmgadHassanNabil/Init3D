cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
	float3 lightDirection;
	float padding;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos	:	SV_POSITION;
	float2 TexCoord :	TEXCOORD;
	float3 Normal	:	NORMAL;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL)
{
	VS_OUTPUT output;

	output.Pos = mul(inPos, WVP);
	output.Normal = mul(inNormal, World);
	output.TexCoord = inTexCoord;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	input.Normal = normalize(input.Normal);

	float diffuse = dot(input.Normal, lightDirection) * 2;
	float4 color = saturate(ObjTexture.Sample(ObjSamplerState, input.TexCoord) * diffuse);
	color.a = 1;
	return color;
}
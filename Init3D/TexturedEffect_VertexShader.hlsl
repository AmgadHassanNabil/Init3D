cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
	bool isSelected;
};

struct VS_OUTPUT
{
	float4 Pos	:	SV_POSITION;
	float2 TexCoord :	TEXCOORD;
	float3 Normal	:	NORMAL;
	bool isSelected : SELECTED;
};

VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL)
{
	VS_OUTPUT output;

	output.Pos = mul(inPos, WVP);
	output.Normal = mul(inNormal, World);
	output.TexCoord = inTexCoord;
	output.isSelected = isSelected;

	return output;
}
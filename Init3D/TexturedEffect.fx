struct Light
{
	float3 dir;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
};
cbuffer cbPerFrame
{
	Light light;
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

	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);

	float3 finalColor;

	finalColor = diffuse * light.ambient;
	finalColor += saturate(dot(light.dir, input.Normal) * light.diffuse * diffuse);

	return float4(finalColor, diffuse.a);
}
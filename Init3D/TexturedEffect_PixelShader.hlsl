struct Light
{
	float3 dir;
	float4 ambient;
	float4 diffuse;
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
	bool isSelected : SELECTED;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	input.Normal = normalize(input.Normal);

	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);

	float3 finalColor;

	finalColor = diffuse * light.ambient;
	finalColor += saturate(dot(light.dir, input.Normal) * light.diffuse * diffuse);

	if (input.isSelected) finalColor += 0.3f;

	return float4(finalColor, diffuse.a);
}
#define MAX_TEXTURES 4

Texture2D ObjTexture[MAX_TEXTURES];
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 texCoords : TEXCOORD;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 endColor = 0;
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		endColor += ObjTexture[i].Sample(ObjSamplerState, input.texCoords);
	}
	return endColor;
}
Texture2D particleTexture;
SamplerState linearSampler;

struct VS
{
	float4 position				: SV_POSITION;
	float4 inceptionPosition	: INCEPTIONPOSITION;
	float2 texCoord				: TEXCOORD;
	float4 random				: RANDOM;
};

float4 main(VS input) : SV_TARGET
{
	return particleTexture.Sample(linearSampler, input.texCoord);
}
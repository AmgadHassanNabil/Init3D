#include "ParticleSystemHeader.hlsli"

Texture2DArray gTexArray;

float4 main(GeoOut pin) : SV_TARGET
{
	return gTexArray.Sample(samLinear, float3(pin.Tex, 0))*pin.Color;
}
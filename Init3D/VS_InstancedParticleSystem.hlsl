#define MAX_PARTICLE_SYSTEMS 8

cbuffer cbPerScene
{
	float4x4 WVP;

	float billWidth;
	float billHeight;
	float2 endSize;

	float3 camUp;
	float ttl;

	float4 camPos;

	float4 direction[MAX_PARTICLE_SYSTEMS];
	float4 emitPosition[MAX_PARTICLE_SYSTEMS];
	float timeSinceInception[MAX_PARTICLE_SYSTEMS];
};

struct VS
{
	float4 position				: SV_POSITION;
	float4 inceptionPosition	: INCEPTIONPOSITION;
	float2 texCoord				: TEXCOORD;
	float4 random				: RANDOM;
	uint id						: PARTICLESYSTEMID;
};

struct VS_OUT
{
	float4 position				: SV_POSITION;
	float4 inceptionPosition	: INCEPTIONPOSITION;
	float2 texCoord				: TEXCOORD;
	float4 random				: RANDOM;
	float size : SIZE;
};

float ComputeParticleSize(float randomValue, float normalizedAge)
{
	float startSize = lerp(billWidth, billHeight, randomValue);
	float x = endSize.x;
	float y = endSize.y;
	float endSize = lerp(x, y, randomValue);

	float size = lerp(startSize, endSize, normalizedAge);

	return size;
}

VS_OUT main(VS input)
{
	VS_OUT output;
	float age = timeSinceInception[input.id] % (ttl * input.random.y);

	float3 particlePosition = emitPosition[input.id] + input.inceptionPosition + (direction[input.id] * input.random.x * age);
	float3 instancedPosition = particlePosition + input.position;

	output.position = mul(float4(instancedPosition, 1), WVP);
	output.texCoord = input.texCoord;
	output.inceptionPosition = float4(instancedPosition, 1);
	output.random = input.random;

	float normalizedAge = saturate(age / ttl);
	output.size = ComputeParticleSize(input.random.z, normalizedAge);

	return output;
}
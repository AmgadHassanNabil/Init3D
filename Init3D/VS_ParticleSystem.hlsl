cbuffer cbPerScene
{
	float4x4 WVP;
	float billWidth;
	float billHeight;
	float2 pad1;
	float3 camUp;
	float timeSinceInception;
	float4 camPos;
};

cbuffer cbDynamics
{
	float3 direction;
	float ttl;
	float2 endSize;
	float2 pad2;
	/*float ttl;
	float3 Gravity;
	float EndVelocity;
	float4 MinColor;
	float4 MaxColor;

	float2 RotateSpeed;
	float2 StartSize;
	float2 EndSize;*/
};

struct VS
{
	float4 position				: SV_POSITION;
	float4 inceptionPosition	: INCEPTIONPOSITION;
	float2 texCoord				: TEXCOORD;
	float4 random				: RANDOM;
};

struct VS_OUT
{
	float4 position				: SV_POSITION;
	float4 inceptionPosition	: INCEPTIONPOSITION;
	float2 texCoord				: TEXCOORD;
	float4 random				: RANDOM;
	float size					: SIZE;
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
	float age = timeSinceInception % (ttl * input.random.y);

	float3 particlePosition = input.inceptionPosition + (direction * input.random.x * age);
	float3 instancedPosition = particlePosition + input.position;

	output.position = mul(float4(instancedPosition, 1), WVP);
	output.texCoord = input.texCoord;
	output.inceptionPosition = float4(instancedPosition, 1);
	output.random = input.random;

	float normalizedAge = saturate(age / ttl);
	output.size = ComputeParticleSize(input.random.y, normalizedAge);

	return output;
}
/*
float4 ComputeParticlePosition(float3 position, float3 velocity,
	float age, float normalizedAge)
{
	float startVelocity = length(velocity);

	float endVelocity = startVelocity * EndVelocity;

	float velocityIntegral = startVelocity * normalizedAge +
		(endVelocity - startVelocity) * normalizedAge *
		normalizedAge / 2;

	position += normalize(velocity) * velocityIntegral * Duration;

	position += Gravity * age * normalizedAge;

	return mul(mul(float4(position, 1), View), Projection);
}*/


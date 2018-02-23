#include "ParticleSystemHeader.hlsli"

VertexOut main(Particle vin)
{
	VertexOut vout;

	float t = vin.Age;

	// constant acceleration equation
	vout.PosW = float4(0.5f*t*t*gAccelW + t * vin.InitialVelW + vin.InitialPosW, 1);

	// fade color with time
	float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
	vout.Color = float4(1.0f, 1.0f, 1.0f, opacity);

	vout.SizeW = vin.SizeW;
	vout.Type = vin.Type;

	return vout;
}
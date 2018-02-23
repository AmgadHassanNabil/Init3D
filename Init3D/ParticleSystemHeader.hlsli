#define PT_EMITTER 0
#define PT_FLARE 1

cbuffer cbPerFrame
{
	float3 gEyePosW;

	// for when the emit position/direction is varying
	float3 gEmitPosW;
	float3 gEmitDirW;

	float gGameTime;
	float gTimeStep;
	float4x4 gViewProj;
};

cbuffer cbFixed
{
	// Net constant acceleration used to accerlate the particles.
	float3 gAccelW = { 0.0f, 7.8f, 0.0f };

	// Texture coordinates used to stretch texture over quad 
	// when we expand point particle into a quad.
	float2 gQuadTexC[4] =
	{
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
	};
};


struct Particle
{
	float4 InitialPosW : SV_POSITION;
	float3 InitialVelW : VELOCITY;
	float2 SizeW       : SIZE;
	float Age : AGE;
	uint Type          : TYPE;
};

struct GeoOut
{
	float4 PosH  : SV_Position;
	float4 Color : COLOR;
	float2 Tex   : TEXCOORD;
};

struct VertexOut
{
	float4 PosW  : SV_POSITION;
	float2 SizeW : SIZE;
	float4 Color : COLOR;
	uint   Type  : TYPE;
};

SamplerState samLinear;
#pragma once
#include <DirectXMath.h>

#include "Effect.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "Vertices.h"

using namespace DirectX;

class ParticleEffect :
	public Effect
{
public:
	struct cbPerObject
	{
		XMMATRIX WVP;

		float billWidth;
		float billHeight;
		XMFLOAT2 pad1;

		XMFLOAT3 camUp;
		float timeSinceInception;

		XMFLOAT4 camPos;

		XMFLOAT3 direction;
		float ttl;

		XMFLOAT2 endSize;
		XMFLOAT2 pad2;

		XMFLOAT3 emitPosition;
		float pad3;

	} objectConstantBuffer;
	//struct cbDynamics
	//{
	//	XMFLOAT3 direction;
	//	float ttl;
	//	XMFLOAT2 endSize;
	//	XMFLOAT2 rotateSpeed;
	//} objectDynamics;


	VertexShader* drawVS;
	PixelShader* drawPS;
	GeometryShader* drawGS;

	static HRESULT createParticleEffect(ID3D11Device * d3d11Device, ParticleEffect & particleEffect);

	void applyDraw();
	void release();

	ParticleEffect();
	~ParticleEffect();
};


#pragma once
#include "VertexShader.h"
#include "PixelShader.h"
#include "Effect.h"
#include <DirectXMath.h>

using namespace DirectX;

class TexturedEffect:public Effect
{
	struct Light
	{
		Light()
		{
			ZeroMemory(this, sizeof(Light));
		}
		XMFLOAT3 lightDirection;
		float pad;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
	};


public:
	struct cbPerFrame
	{
		Light light;
	} frameConfigurations;
	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;
	} cbPerObj;


	VertexShader* vs;
	PixelShader* ps;

	void release();
	void apply() override;

	TexturedEffect(ID3D11Device* d3d11Device);
	~TexturedEffect();
};


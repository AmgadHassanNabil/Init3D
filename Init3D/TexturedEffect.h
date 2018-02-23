#pragma once
#include "VertexShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>

using namespace DirectX;

class TexturedEffect
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

	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;
	};

	

	cbPerObject cbPerObj;
public:
	struct cbPerFrame
	{
		Light light;
	} frameConfigurations;

	VertexShader vs;
	PixelShader ps;

	TexturedEffect();
	~TexturedEffect();
};


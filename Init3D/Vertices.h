#pragma once

#include "Headers.h"

using namespace DirectX;

struct VertexPositionColor
{
	XMFLOAT3 pos;
	XMFLOAT4 color;

	VertexPositionColor(float x, float y, float z, float r, float g, float b)
		: pos(x, y, z), color(r, g, b, 1.0f)
	{	}

	const static D3D11_INPUT_ELEMENT_DESC layout[2];

	const static UINT numElements = 2;
};

struct VertexPositionTexture
{
	XMFLOAT3 pos;
	XMFLOAT2 texCoords;

	VertexPositionTexture(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoords(u, v)
	{	}

	const static D3D11_INPUT_ELEMENT_DESC layout[2];

	const static UINT numElements = 2;
};

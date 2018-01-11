#pragma once

#include "Headers.h"

using namespace DirectX;

struct VertexPositionColor
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	VertexPositionColor()
	{	}
	VertexPositionColor(XMFLOAT3 _pos, XMFLOAT4 _color) 
	{
		this->pos = _pos;
		this->color = _color;
	}
	VertexPositionColor(float x, float y, float z, float r, float g, float b)
		: pos(x, y, z), color(r, g, b, 1.0f)
	{	}

	const static D3D11_INPUT_ELEMENT_DESC layout[2];

	const static UINT numElements = ARRAYSIZE(layout);
};

struct VertexPositionTexture
{
	XMFLOAT3 pos;
	XMFLOAT2 texCoords;

	VertexPositionTexture(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoords(u, v)
	{	}

	const static D3D11_INPUT_ELEMENT_DESC layout[2];

	const static UINT numElements = ARRAYSIZE(layout);
};

struct VertexPositionNormalTexture
{
	XMFLOAT3 pos;
	XMFLOAT2 texCoords;
	XMFLOAT3 normal;

	VertexPositionNormalTexture()
	{	}
	VertexPositionNormalTexture(XMFLOAT3 pos,
						XMFLOAT2 texCoords,
						XMFLOAT3 normal)
		:pos(pos), texCoords(texCoords), normal(normal)
	{	}
	VertexPositionNormalTexture(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), texCoords(u, v), normal(nx, ny, nz)
	{	}

	const static D3D11_INPUT_ELEMENT_DESC layout[3];

	const static UINT numElements = ARRAYSIZE(layout);
};

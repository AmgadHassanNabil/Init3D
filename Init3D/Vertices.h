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

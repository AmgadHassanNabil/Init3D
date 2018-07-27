#pragma once
#include "Effect.h"
#include "VertexShader.h"
#include "Vertices.h"
#include "PixelShader.h"
#include "Effect.h"
#include <DirectXMath.h>

using namespace DirectX;

class SpriteEffect :
	public Effect
{
public:
	SpriteEffect();
	~SpriteEffect();

	VertexShader vs;
	PixelShader ps;

	void release();

	static HRESULT createSpriteEffect(_In_ ID3D11Device* d3d11Device, _Outref_ SpriteEffect& texturedEFfect);
};


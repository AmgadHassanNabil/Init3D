#include "SpriteEffect.h"



void SpriteEffect::release()
{
	vs.release();
	ps.release();
	vertLayout->Release();
}


HRESULT SpriteEffect::createSpriteEffect(ID3D11Device * d3d11Device, SpriteEffect & spriteEffect)
{

	HRESULT hr;
	hr = VertexShader::loadAndCreateVertexShader(L"SpriteEffect_VertexShader.hlsl", d3d11Device, spriteEffect.vs);
	if (FAILED(hr)) return hr;
	hr = PixelShader::loadAndCreatePixelShader(L"SpriteEffect_PixelShader.hlsl", d3d11Device, spriteEffect.ps);
	if (FAILED(hr)) return hr;

	hr = d3d11Device->CreateInputLayout(VertexPositionTexture::layout, VertexPositionTexture::numElements,
		spriteEffect.vs.Buffer->GetBufferPointer(), spriteEffect.vs.Buffer->GetBufferSize(), &spriteEffect.vertLayout);
	if (FAILED(hr)) return hr;

	return S_OK;
}

SpriteEffect::SpriteEffect()
{
}


SpriteEffect::~SpriteEffect()
{
}

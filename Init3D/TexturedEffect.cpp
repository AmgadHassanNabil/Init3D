#include "TexturedEffect.h"



void TexturedEffect::apply()
{
	
}

TexturedEffect::TexturedEffect() : Effect(L"TexturedEffect.fx")
{
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	HRESULT hr = AMD3D->d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);
}


TexturedEffect::~TexturedEffect()
{
}

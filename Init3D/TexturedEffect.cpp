#include "TexturedEffect.h"



void TexturedEffect::release()
{
	vs->release();
	delete vs;
	ps->release();
	delete ps;
	cbPerFrameBuffer->Release();
}

void TexturedEffect::apply()
{
	pFrameData = &frameConfigurations;
	Effect::apply();
}

TexturedEffect::TexturedEffect(ID3D11Device* d3d11Device)
{
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	HRESULT hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);

	vs = new VertexShader(L"TexturedEffect_VertexShader.hlsl", d3d11Device);
	ps = new PixelShader(L"TexturedEffect_PixelShader.hlsl", d3d11Device);

	this->sizeOfPerObjectCB = sizeof(cbPerObject);
}


TexturedEffect::~TexturedEffect()
{
}

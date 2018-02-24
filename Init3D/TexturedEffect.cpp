#include "TexturedEffect.h"



void TexturedEffect::release()
{
	vs.release();
	ps.release();
	cbPerFrameBuffer->Release();
	vertLayout->Release();
}

void TexturedEffect::apply()
{
	pFrameData = &frameConfigurations;
	Effect::apply();
}

HRESULT TexturedEffect::createTexturedEffect(ID3D11Device * d3d11Device, TexturedEffect & texturedEFfect)
{
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	HRESULT hr = d3d11Device->CreateBuffer(&cbbd, NULL, &texturedEFfect.cbPerFrameBuffer);
	if (FAILED(hr)) return hr;
	hr = VertexShader::loadAndCreateVertexShader(L"TexturedEffect_VertexShader.hlsl", d3d11Device, texturedEFfect.vs);
	if (FAILED(hr)) return hr;
	hr = PixelShader::loadAndCreatePixelShader(L"TexturedEffect_PixelShader.hlsl", d3d11Device, texturedEFfect.ps);
	if (FAILED(hr)) return hr;

	texturedEFfect.sizeOfPerObjectCB = sizeof(cbPerObject);

	hr = d3d11Device->CreateInputLayout(VertexPositionNormalTexture::layout, VertexPositionNormalTexture::numElements, 
		texturedEFfect.vs.Buffer->GetBufferPointer(), texturedEFfect.vs.Buffer->GetBufferSize(), &texturedEFfect.vertLayout);
	if (FAILED(hr)) return hr;

	return S_OK;
}

TexturedEffect::TexturedEffect()
{
}


TexturedEffect::~TexturedEffect()
{
}

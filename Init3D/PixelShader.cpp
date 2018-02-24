#include "PixelShader.h"



PixelShader::PixelShader()
{
}

HRESULT PixelShader::loadAndCreatePixelShader(_In_ LPCWSTR srcFile, _In_ ID3D11Device * d3d11Device, _Outref_ PixelShader & ps)
{

	HRESULT hr;

	hr = ps.CompileShader(srcFile, "main", "ps_5_0", &ps.Buffer);
	if (FAILED(hr)) return hr;
	hr = d3d11Device->CreatePixelShader(ps.Buffer->GetBufferPointer(), ps.Buffer->GetBufferSize(), NULL, &ps.PS);
	if (FAILED(hr)) return hr;
	return S_OK;
}

void PixelShader::release()
{
	Shader::release();
	PS->Release();
}


PixelShader::~PixelShader()
{
}

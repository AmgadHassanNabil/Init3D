#include "PixelShader.h"



PixelShader::PixelShader(LPCWSTR srcFile, ID3D11Device* d3d11Device)
{
	HRESULT hr;

	hr = CompileShader(srcFile, "main", "ps_5_0", &Buffer);
	hr = d3d11Device->CreatePixelShader(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), NULL, &PS);
}

void PixelShader::release()
{
	Shader::release();
	PS->Release();
}


PixelShader::~PixelShader()
{
}

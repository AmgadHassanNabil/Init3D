#include "PixelShader.h"



PixelShader::PixelShader(LPCWSTR srcFile)
{
	HRESULT hr;

	hr = CompileShader(srcFile, "main", "ps_5_0", &Buffer);
	hr = AMD3D->d3d11Device->CreatePixelShader(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), NULL, &PS);
}

void PixelShader::release()
{
	((Shader*)this)->release();
}


PixelShader::~PixelShader()
{
}

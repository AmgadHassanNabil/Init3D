#include "VertexShader.h"



VertexShader::VertexShader(LPCWSTR srcFile, ID3D11Device* d3d11Device)
{
	HRESULT hr;

	hr = CompileShader(srcFile, "main", "vs_5_0", &Buffer);
	hr = d3d11Device->CreateVertexShader(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), NULL, &VS);
}

void VertexShader::release()
{
	Shader::release();
	VS->Release();
}


VertexShader::~VertexShader()
{
}

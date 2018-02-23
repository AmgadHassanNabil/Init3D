#include "VertexShader.h"



VertexShader::VertexShader(LPCWSTR srcFile)
{
	HRESULT hr;

	hr = CompileShader(srcFile, "main", "vs_5_0", &Buffer);
	hr = AMD3D->d3d11Device->CreateVertexShader(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), NULL, &VS);
}

void VertexShader::release()
{
	((Shader*)this)->release();
}


VertexShader::~VertexShader()
{
}

#include "VertexShader.h"



VertexShader::VertexShader()
{
	VS = nullptr;
}

HRESULT VertexShader::loadAndCreateVertexShader(_In_ LPCWSTR srcFile, _In_ ID3D11Device * d3d11Device, _Outref_ VertexShader & vs)
{
	HRESULT hr;

	hr = vs.CompileShader(srcFile, "main", "vs_5_0", &vs.Buffer);
	if (FAILED(hr)) return hr;
	hr = d3d11Device->CreateVertexShader(vs.Buffer->GetBufferPointer(), vs.Buffer->GetBufferSize(), NULL, &vs.VS);
	if (FAILED(hr)) return hr;

	return S_OK;
}

void VertexShader::release()
{
	Shader::release();
	if(VS)
		VS->Release();
}


VertexShader::~VertexShader()
{
}

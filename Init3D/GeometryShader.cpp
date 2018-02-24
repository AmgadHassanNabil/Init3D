#include "GeometryShader.h"



GeometryShader::GeometryShader()
{
}


GeometryShader::~GeometryShader()
{
}

HRESULT GeometryShader::loadAndCreateGeometryShader(_In_ LPCWSTR srcFile, _In_ ID3D11Device * d3d11Device, _Outref_ GeometryShader & gs)
{
	HRESULT hr;

	hr = gs.CompileShader(srcFile, "main", "gs_5_0", &gs.Buffer);
	if (FAILED(hr)) return hr;
	hr = d3d11Device->CreateGeometryShader(gs.Buffer->GetBufferPointer(), gs.Buffer->GetBufferSize(), NULL, &gs.GS);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT GeometryShader::loadAndCreateGeometryShaderStreamOut(LPCWSTR srcFile, const D3D11_SO_DECLARATION_ENTRY pDecl[], ID3D11Device * d3d11Device, GeometryShader & gs)
{
	HRESULT hr;

	hr = gs.CompileShader(srcFile, "main", "gs_5_0", &gs.Buffer);
	if (FAILED(hr)) return hr;
	hr = d3d11Device->CreateGeometryShaderWithStreamOutput(gs.Buffer->GetBufferPointer(), gs.Buffer->GetBufferSize(), pDecl, sizeof(pDecl), NULL, 0, 0, NULL, &gs.GS);
	if (FAILED(hr)) return hr;

	return S_OK;
}

void GeometryShader::release()
{
	Shader::release();
	GS->Release();
}
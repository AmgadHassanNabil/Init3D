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

	//ID3D11ShaderReflection* pReflector = NULL;
	//D3DReflect(gs.Buffer->GetBufferPointer(), gs.Buffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);

	//ID3D11ShaderReflectionConstantBuffer* cb = pReflector->GetConstantBufferByIndex(0);
	//
	//D3D11_SHADER_BUFFER_DESC cbDesc;
	//cb->GetDesc(&cbDesc);
	//int y = 0;

	//for (int i = 0; i < cbDesc.Variables; i++)
	//{
	//	ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(i);

	//	ID3D11ShaderReflectionType* type = var->GetType();

	//	D3D11_SHADER_VARIABLE_DESC varDesc;
	//	var->GetDesc(&varDesc);
	//	int x = varDesc.Size;
	//	y += x;
	//}
	



	return S_OK;
}

HRESULT GeometryShader::loadAndCreateGeometryShaderStreamOut(LPCWSTR srcFile, const D3D11_SO_DECLARATION_ENTRY pDecl[], int numberOfSODeclElements, ID3D11Device * d3d11Device, GeometryShader & gs)
{
	HRESULT hr;

	hr = gs.CompileShader(srcFile, "main", "gs_5_0", &gs.Buffer);
	if (FAILED(hr)) return hr;
	hr = d3d11Device->CreateGeometryShaderWithStreamOutput(gs.Buffer->GetBufferPointer(), gs.Buffer->GetBufferSize(), pDecl, numberOfSODeclElements, NULL, 0, 0, NULL, &gs.GS);
	if (FAILED(hr)) return hr;

	return S_OK;
}

void GeometryShader::release()
{
	Shader::release();
	GS->Release();
}
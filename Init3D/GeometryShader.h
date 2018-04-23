#pragma once
#include "Shader.h"
class GeometryShader :
	public Shader
{
public:
	ID3D11GeometryShader * GS;

	GeometryShader();
	~GeometryShader();

	static HRESULT loadAndCreateGeometryShader(LPCWSTR srcFile, ID3D11Device * d3d11Device, GeometryShader & vs);
	static HRESULT loadAndCreateGeometryShaderStreamOut(LPCWSTR srcFile, const D3D11_SO_DECLARATION_ENTRY pDecl[], int numberOfSODeclElements, ID3D11Device * d3d11Device, GeometryShader & vs);

	void release() override;
};


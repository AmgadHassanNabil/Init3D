#pragma once
#include "Shader.h"
class VertexShader :
	public Shader
{
public:
	VertexShader();
	ID3D11VertexShader * VS;

	static HRESULT loadAndCreateVertexShader(LPCWSTR srcFile, ID3D11Device* d3d11Device, VertexShader & vs);
	void release() override;

	~VertexShader();
};


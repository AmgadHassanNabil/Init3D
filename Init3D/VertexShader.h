#pragma once
#include "Shader.h"
class VertexShader :
	public Shader
{
public:
	ID3D11VertexShader * VS;

	VertexShader(LPCWSTR srcFile, ID3D11Device* d3d11Device);

	void release() override;

	~VertexShader();
};


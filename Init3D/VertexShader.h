#pragma once
#include "Shader.h"
class VertexShader :
	public Shader
{
public:
	ID3D11VertexShader * VS;

	VertexShader(LPCWSTR srcFile);

	void release() override;

	~VertexShader();
};


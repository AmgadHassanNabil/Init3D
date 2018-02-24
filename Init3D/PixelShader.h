#pragma once
#include "Shader.h"
class PixelShader :
	public Shader
{
public:
	ID3D11PixelShader * PS;

	PixelShader(LPCWSTR srcFile, ID3D11Device* d3d11Device);
	void release() override;
	~PixelShader();
};


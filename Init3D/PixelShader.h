#pragma once
#include "Shader.h"
class PixelShader :
	public Shader
{
public:
	ID3D11PixelShader * PS;

	PixelShader(LPCWSTR srcFile);
	void release() override;
	~PixelShader();
};


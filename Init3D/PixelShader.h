#pragma once
#include "Shader.h"
class PixelShader :
	public Shader
{
public:
	PixelShader();
	ID3D11PixelShader * PS;


	static HRESULT loadAndCreatePixelShader(LPCWSTR srcFile, ID3D11Device* d3d11Device, PixelShader & ps);

	void release() override;
	~PixelShader();
};


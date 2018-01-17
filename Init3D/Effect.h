#pragma once
#include <d3d11.h>
#include <D3Dcompiler.h>
#include "Direct3D.h"

class Effect
{


public:
	ID3DBlob* VS_Buffer = NULL;
	ID3DBlob* PS_Buffer = NULL;
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;

	HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob);

	Effect();
	Effect(LPCWSTR srcFile);
	void release();
	~Effect();
};


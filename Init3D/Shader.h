#pragma once
#include <D3Dcompiler.h>
#include "Direct3D.h"

class Shader
{
protected:
	HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob);
public:
	ID3DBlob* Buffer = NULL;
	

	Shader();
	virtual void release();
	~Shader();
};


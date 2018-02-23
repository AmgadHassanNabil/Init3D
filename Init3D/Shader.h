#pragma once
#include <d3d11.h>
#include <D3Dcompiler.h>
#include "Direct3D.h"

class Shader
{
protected:
	UINT sizeOfPerObjectCB;
	HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob);
public:
	ID3DBlob* Buffer = NULL;
	
	
	ID3D11Buffer* cbPerFrameBuffer;
	void * pFrameData;

	inline const UINT getSizeOfPerObjectCB() { return sizeOfPerObjectCB;  }
	
	//virtual void apply();

	Shader();
	virtual void release();
	~Shader();
};


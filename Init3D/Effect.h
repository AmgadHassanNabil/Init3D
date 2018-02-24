#pragma once
#include "Direct3D.h"

class Effect
{

protected:
	UINT sizeOfPerObjectCB;
	ID3D11InputLayout * vertLayout;

public:

	ID3D11Buffer * cbPerFrameBuffer;
	void * pFrameData;

	inline const UINT getSizeOfPerObjectCB() { return sizeOfPerObjectCB; }
	inline ID3D11InputLayout * getInputLayout() { return vertLayout; }
	virtual void apply();
	
	Effect();
	~Effect();
};


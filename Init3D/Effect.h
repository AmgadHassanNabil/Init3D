#pragma once
#include "Direct3D.h"

class Effect
{

protected:
	UINT sizeOfPerObjectCB;

public:


	ID3D11Buffer * cbPerFrameBuffer;
	void * pFrameData;

	inline const UINT getSizeOfPerObjectCB() { return sizeOfPerObjectCB; }
	virtual void apply();
	
	Effect();
	~Effect();
};


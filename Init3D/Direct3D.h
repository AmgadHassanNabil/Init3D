#pragma once
#include "Headers.h"

class Direct3D
{
private:
	static Direct3D* instance;

	Direct3D();
	Direct3D(const Direct3D&);
	Direct3D& operator=(const Direct3D&);
	~Direct3D();

public:	
	IDXGISwapChain * SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RasterizerState* WireFrame;

	static Direct3D* getInstance();
	bool initialize(HWND, HINSTANCE, const int, const int);
	void shutdown();
};


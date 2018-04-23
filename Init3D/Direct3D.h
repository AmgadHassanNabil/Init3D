#pragma once
#include <windows.h>
#include <d3d11.h>
#include <CommonStates.h>

using namespace DirectX;

#define	AMD3D			Direct3D::getInstance()

class Direct3D
{
private:
	static Direct3D* instance;
	CommonStates * commonStates;

	Direct3D();
	Direct3D(const Direct3D&);
	Direct3D& operator=(const Direct3D&);
	~Direct3D();


	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_defaultBlendingState;

	ID3D11BlendState* m_additiveBlendingState;

	HRESULT createStockStates();

public:	
	IDXGISwapChain * SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RasterizerState* defaultRasterizerState;

	inline static Direct3D* getInstance()
	{
		if (instance == nullptr)
			instance = new Direct3D();
		return instance;
	}
	bool initialize(HWND, HINSTANCE, const int, const int);
	void shutdown();

	inline void enableDefaultBlending()
	{
		float blendFactor[4];

		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;


		d3d11DevCon->OMSetBlendState(commonStates->Opaque(), blendFactor, 0xffffffff);
	}
	inline void enableAlphaBlending()
	{
		float blendFactor[4];

		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;

		d3d11DevCon->OMSetBlendState(commonStates->AlphaBlend(), blendFactor, 0xffffffff);
	}
	inline void enableAdditiveBlending()
	{
		float blendFactor[4];

		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;

		d3d11DevCon->OMSetBlendState(commonStates->Additive(), blendFactor, 0xffffffff);
	}
	 
	inline void defaultDepth()
	{
		d3d11DevCon->OMSetDepthStencilState(commonStates->DepthDefault(), 0);
	}
	inline void disableDepth()
	{
		d3d11DevCon->OMSetDepthStencilState(commonStates->DepthNone(), 0);
	}
	inline void noDepthWrite()
	{
		d3d11DevCon->OMSetDepthStencilState(commonStates->DepthRead(), 0);
	}
};


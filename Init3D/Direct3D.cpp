#include "Direct3D.h"

//#include<stdio.h>

Direct3D* Direct3D::instance;

Direct3D::Direct3D()
{
	ZeroMemory(this, sizeof(this));
}

Direct3D * Direct3D::getInstance()
{
	if (instance == nullptr)
		instance = new Direct3D();
	return instance;
}

bool Direct3D::initialize(HWND hwnd, HINSTANCE hInstance, const int width, const int height)
{
	HRESULT hr;

	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);


	/*for (int i = 1; i < 33; i++)
	{
		UINT quality = 0;
		hr = d3d11Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, i, &quality);

		if (quality > 1)
		{
			char str[50];
			sprintf_s(str, "Multisampling is working with: %d and quality: %d", i, quality);
			OutputDebugStringA(str);
		}
	}*/

	if (FAILED(hr))
		return false;

	ID3D11Texture2D* BackBuffer;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	if (FAILED(hr))
		return false;

	hr = d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);

	if (FAILED(hr))
		return false;

	BackBuffer->Release();
	
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 4;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC pD;
	ZeroMemory(&pD, sizeof(pD));
	pD.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	d3d11Device->CreateDepthStencilView(depthStencilBuffer, &pD, &depthStencilView);

	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	Direct3D::getInstance()->d3d11DevCon->RSSetViewports(1, &viewport);

	/*D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_BACK;
	wfdesc.MultisampleEnable = TRUE;
	wfdesc.AntialiasedLineEnable = TRUE;
	hr = d3d11Device->CreateRasterizerState(&wfdesc, &WireFrame);
	d3d11DevCon->RSSetState(WireFrame);*/ 

	return true;
}

void Direct3D::shutdown()
{
	this->SwapChain->Release();
	this->d3d11Device->Release();
	this->d3d11DevCon->Release();
	this->depthStencilView->Release();
	this->depthStencilBuffer->Release();
	//this->WireFrame->Release();
	delete this;
}

Direct3D::~Direct3D()
{
}

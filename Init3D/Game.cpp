#include "Game.h"

Game* Game::instance;

HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob);

Game::Game()
{
	ZeroMemory(this, sizeof(this));
}

bool Game::initialize(UINT width, UINT height)
{
	this->width = width;
	this->height = height;

	VertexPositionColor quadVerticies[] =
	{
		VertexPositionColor(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f),
		VertexPositionColor(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f),
		VertexPositionColor(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f),
		VertexPositionColor(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f),
		VertexPositionColor(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f),
		VertexPositionColor(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f),
		VertexPositionColor(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f),
		VertexPositionColor(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f),
	};

	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	HRESULT hr;
	
	hr = CompileShader(L"Effects.fx", "VS", "vs_5_0", &VS_Buffer);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "VertexShader Loading - Failed", "Error");
	hr = CompileShader(L"Effects.fx", "PS", "ps_5_0", &PS_Buffer);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "PixelShader Loading - Failed", "Error");

	hr = D3DINST->d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "VertexShader Creating - Failed", "Error");
	hr = D3DINST->d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "PixelShader Creating - Failed", "Error");

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * 8;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = quadVerticies;
	hr = D3DINST->d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &quadVertBuffer);

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	hr = D3DINST->d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &quadIndexBuffer);

	D3DINST->d3d11Device->CreateInputLayout(VertexPositionColor::layout, VertexPositionColor::numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);	

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	hr = D3DINST->d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f);
	

	return true;
}

float yRotate = -XM_2PI;
//float zCamOffset = 8.0f;
//int zCamOffsetSign = 1;

void Game::update()
{
	camPosition = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	World = XMMatrixRotationY(yRotate);

	yRotate += (XM_2PI / 5000.0f);
	//zCamOffset += (zCamOffsetSign * 0.001f);
	//if (zCamOffset > 5 || zCamOffset < 0.5f)
	//	zCamOffsetSign *= -1;
}

void Game::draw()
{
	D3DINST->d3d11DevCon->ClearRenderTargetView(D3DINST->renderTargetView, bgColor);
	D3DINST->d3d11DevCon->ClearDepthStencilView(D3DINST->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


#ifdef _WIN64
	WVP = XMMatrixMultiply(World, XMMatrixMultiply(camView, camProjection));
#else
	WVP = World * camView * camProjection
#endif
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	D3DINST->d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	D3DINST->d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	D3DINST->d3d11DevCon->VSSetShader(VS, NULL, NULL);
	D3DINST->d3d11DevCon->PSSetShader(PS, NULL, NULL);

	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	D3DINST->d3d11DevCon->IASetVertexBuffers(0, 1, &quadVertBuffer, &stride, &offset);
	D3DINST->d3d11DevCon->IASetIndexBuffer(quadIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3DINST->d3d11DevCon->IASetInputLayout(vertLayout);

	D3DINST->d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DINST->d3d11DevCon->DrawIndexed(36, 0, 0);

	D3DINST->SwapChain->Present(0, 0);
}

void Game::release()
{
	VS_Buffer->Release();
	PS_Buffer->Release();
	quadVertBuffer->Release();
	quadIndexBuffer->Release();
	vertLayout->Release();
	VS->Release();
	PS->Release();
	cbPerObjectBuffer->Release();
	delete this;
}

Game * Game::getInstance()
{
	if (instance == nullptr)
		instance = new Game();
	return instance;
}

Game::~Game()
{
}

HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	/*const D3D_SHADER_MACRO defines[] =
	{
	"EXAMPLE_DEFINE", "1",
	NULL, NULL
	};*/

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}
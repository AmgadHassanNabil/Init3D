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

	VertexPositionTexture cubeVerticies[] =
	{
		// Front Face
		VertexPositionTexture(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		VertexPositionTexture(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		VertexPositionTexture(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		VertexPositionTexture(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Back Face
		VertexPositionTexture(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		VertexPositionTexture(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		VertexPositionTexture(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		VertexPositionTexture(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

		// Top Face
		VertexPositionTexture(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		VertexPositionTexture(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		VertexPositionTexture(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		VertexPositionTexture(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		// Bottom Face
		VertexPositionTexture(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		VertexPositionTexture(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		VertexPositionTexture(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		VertexPositionTexture(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

		// Left Face
		VertexPositionTexture(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		VertexPositionTexture(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		VertexPositionTexture(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		VertexPositionTexture(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Right Face
		VertexPositionTexture(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		VertexPositionTexture(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		VertexPositionTexture(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
		VertexPositionTexture(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
	};

	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	HRESULT hr;
	
	hr = CompileShader(L"TexturedEffect.fx", "VS", "vs_5_0", &VS_Buffer);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "VertexShader Loading - Failed", "Error");
	hr = CompileShader(L"TexturedEffect.fx", "PS", "ps_5_0", &PS_Buffer);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "PixelShader Loading - Failed", "Error");

	hr = AMD3D->d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "VertexShader Creating - Failed", "Error");
	hr = AMD3D->d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "PixelShader Creating - Failed", "Error");

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionTexture) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = cubeVerticies;
	hr = AMD3D->d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &cubeVertBuffer);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Vertex Buffer Creation - Failed", "Error");

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	hr = AMD3D->d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &cubeIndexBuffer);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Index Buffer Creation - Failed", "Error");

	hr = AMD3D->d3d11Device->CreateInputLayout(VertexPositionTexture::layout, VertexPositionTexture::numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Input Layout Creation - Failed", "Error");

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	hr = AMD3D->d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Constant Buffer Creation - Failed", "Error");

	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f);
	
	//D3D11_SAMPLER_DESC sampDesc;
	//ZeroMemory(&sampDesc, sizeof(sampDesc));
	//sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//sampDesc.MinLOD = 0;
	//sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	//hr = AMD3D->d3d11Device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);
	//SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Sampler State Creation - Failed", "Error");

	hr = CreateWICTextureFromFile(AMD3D->d3d11Device, L"braynzar.jpg", nullptr, &CubesTexture);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Loading Texture - Failed", "Error");

	return true;
}

float yRotate = -XM_2PI;
//float zCamOffset = 8.0f;
//int zCamOffsetSign = 1;

void Game::update(const double& time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[])
{
	camPosition = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	World = XMMatrixRotationY(yRotate);


	if(mouseCurrState.rgbButtons[0] & 0x80)
		yRotate += (XM_2PI) * time;
	if (mouseCurrState.rgbButtons[1] & 0x80)
		yRotate -= (XM_2PI) * time;
	//zCamOffset += (zCamOffsetSign * 0.001f);
	//if (zCamOffset > 5 || zCamOffset < 0.5f)
	//	zCamOffsetSign *= -1;
}

void Game::draw(const int& fps)
{
	AMD3D->d3d11DevCon->ClearRenderTargetView(AMD3D->renderTargetView, bgColor);
	AMD3D->d3d11DevCon->ClearDepthStencilView(AMD3D->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


#ifdef _WIN64
	WVP = XMMatrixMultiply(World, XMMatrixMultiply(camView, camProjection));
#else
	WVP = World * camView * camProjection
#endif
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	AMD3D->d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	AMD3D->d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	AMD3D->d3d11DevCon->PSSetShaderResources(0, 1, &CubesTexture);
	//AMD3D->d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);

	AMD3D->d3d11DevCon->VSSetShader(VS, NULL, NULL);
	AMD3D->d3d11DevCon->PSSetShader(PS, NULL, NULL);

	UINT stride = sizeof(VertexPositionTexture);
	UINT offset = 0;
	AMD3D->d3d11DevCon->IASetVertexBuffers(0, 1, &cubeVertBuffer, &stride, &offset);
	AMD3D->d3d11DevCon->IASetIndexBuffer(cubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	AMD3D->d3d11DevCon->IASetInputLayout(vertLayout);

	AMD3D->d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	AMD3D->d3d11DevCon->DrawIndexed(36, 0, 0);

	AMD3D->SwapChain->Present(0, 0);
}

void Game::release()
{
	VS_Buffer->Release();
	PS_Buffer->Release();
	cubeVertBuffer->Release();
	cubeIndexBuffer->Release();
	vertLayout->Release();
	VS->Release();
	PS->Release();
	cbPerObjectBuffer->Release();
	//CubesTexSamplerState->Release();
	CubesTexture->Release();
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
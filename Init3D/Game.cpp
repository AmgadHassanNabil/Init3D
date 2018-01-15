#include "Game.h"

Game* Game::instance;

HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob);

Game::Game()
{
	ZeroMemory(this, sizeof(this));
}

bool Game::loadModel()
{
	XMFLOAT3* positions = NULL;
	DWORD* indiciesF = NULL;
	XMFLOAT3* normals = NULL;
	DWORD numberOfNormals;
	XMFLOAT2* uvs = NULL;

	HRESULT hr = FBXImporter::getInstance()->parseFBX("D:\\Graphics\\SpaceShip.fbx",
		&positions, numberOfVerticies,
		&indiciesF, numberOfIndicies,
		&normals, &uvs, textures, numberOfTextures);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Loading Model - Failed", "Error");

	VertexPositionNormalTexture* modelVerticies = new VertexPositionNormalTexture[numberOfVerticies];

	for (DWORD i = 0; i < numberOfVerticies; i++)
		modelVerticies[i] = VertexPositionNormalTexture(positions[i], uvs[i], normals[i]);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionNormalTexture) * numberOfVerticies;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = modelVerticies;
	hr = AMD3D->d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &cubeVertBuffer);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Vertex Buffer Creation - Failed", "Error");

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * numberOfIndicies;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indiciesF;
	hr = AMD3D->d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &cubeIndexBuffer);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Index Buffer Creation - Failed", "Error");

	hr = AMD3D->d3d11Device->CreateInputLayout(VertexPositionNormalTexture::layout, VertexPositionNormalTexture::numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Input Layout Creation - Failed", "Error");

	delete[] modelVerticies;
	delete[] positions;
	delete[] normals;
	delete[] uvs;
}

bool Game::initialize(UINT width, UINT height)
{
	this->width = width;
	this->height = height;

	VertexPositionNormalTexture cubeVerticies[] =
	{
		// Front Face
		VertexPositionNormalTexture(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		VertexPositionNormalTexture(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		// Back Face
		VertexPositionNormalTexture(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
		VertexPositionNormalTexture(1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f, 1.0f),
		VertexPositionNormalTexture(1.0f,  1.0f, 1.0f, 0.0f, 0.0f,  1.0f,  1.0f, 1.0f),
		VertexPositionNormalTexture(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

		// Top Face
		VertexPositionNormalTexture(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		VertexPositionNormalTexture(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		VertexPositionNormalTexture(1.0f, 1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,  1.0f),
		VertexPositionNormalTexture(1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  1.0f, 1.0f, -1.0f),

		// Bottom Face
		VertexPositionNormalTexture(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		VertexPositionNormalTexture(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		// Left Face
		VertexPositionNormalTexture(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		VertexPositionNormalTexture(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		VertexPositionNormalTexture(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		VertexPositionNormalTexture(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		// Right Face
		VertexPositionNormalTexture(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		VertexPositionNormalTexture(1.0f, -1.0f,  1.0f, 1.0f, 1.0f,1.0f, -1.0f,  1.0f),
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

	//D3D11_BUFFER_DESC vertexBufferDesc;
	//ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBufferDesc.ByteWidth = sizeof(VertexPositionNormalTexture) * 24;
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = 0;
	//vertexBufferDesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA vertexBufferData;

	//ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	//vertexBufferData.pSysMem = cubeVerticies;
	//hr = AMD3D->d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &cubeVertBuffer);
	//SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Vertex Buffer Creation - Failed", "Error");

	//D3D11_BUFFER_DESC indexBufferDesc;
	//ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA iinitData;
	//iinitData.pSysMem = indices;
	//hr = AMD3D->d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &cubeIndexBuffer);
	//SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Index Buffer Creation - Failed", "Error");

	//hr = AMD3D->d3d11Device->CreateInputLayout(VertexPositionNormalTexture::layout, VertexPositionNormalTexture::numElements, VS_Buffer->GetBufferPointer(),
	//	VS_Buffer->GetBufferSize(), &vertLayout);
	//SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Input Layout Creation - Failed", "Error");

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	hr = AMD3D->d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Constant Buffer Creation - Failed", "Error");

	loadModel();

	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f);
	
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 4;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = AMD3D->d3d11Device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);
	SHOW_AND_RETURN_ERROR_ON_FAIL(hr, "Sampler State Creation - Failed", "Error");

	camPosition = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);

	return true;
}

float yRotate = -XM_2PI;
//float zCamOffset = 8.0f;
//int zCamOffsetSign = 1;

float camPosZ = 0;
float camRotY = 0;
void Game::update(const double& time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[])
{
	

	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	World = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(yRotate);


	if (INPUT_DOWN(mouseCurrState.rgbButtons[0]))
		yRotate += (XM_2PI) * time;
	if (INPUT_DOWN(mouseCurrState.rgbButtons[1]))
		yRotate -= (XM_2PI) * time;
	if (INPUT_DOWN(currKeyboardState[DIK_G]))
		camPosZ += time * 4.5f;
	if (INPUT_DOWN(currKeyboardState[DIK_B]))
		camPosZ -= time * 4.5f;
	if (INPUT_DOWN(currKeyboardState[DIK_F]))
		camRotY -= (XM_2PI * time);
	if (INPUT_DOWN(currKeyboardState[DIK_H]))
		camRotY += (XM_2PI * time);

	XMMATRIX camRot = XMMatrixRotationY(camRotY);
	camPosition = XMVector3Transform(camPosition, camRot);
	XMVECTOR directionToOrigin = XMVector3Normalize(camTarget - camPosition);
	camPosition = camPosition + (directionToOrigin * camPosZ);//XMVectorSetZ(camPosition, XMVectorGetZ(camPosition) + camPosZ);

	camRotY = 0;
	camPosZ = 0;
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
	cbPerObj.World = XMMatrixTranspose(World);
	cbPerObj.lightDirection = XMFLOAT3(0.3f, 0.5f, 0.2f);
	AMD3D->d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	AMD3D->d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	AMD3D->d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	AMD3D->d3d11DevCon->PSSetShaderResources(0, 1, &textures[0]);
	AMD3D->d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);

	AMD3D->d3d11DevCon->VSSetShader(VS, NULL, NULL);
	AMD3D->d3d11DevCon->PSSetShader(PS, NULL, NULL);

	UINT stride = sizeof(VertexPositionNormalTexture);
	UINT offset = 0;
	AMD3D->d3d11DevCon->IASetVertexBuffers(0, 1, &cubeVertBuffer, &stride, &offset);
	AMD3D->d3d11DevCon->IASetIndexBuffer(cubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	AMD3D->d3d11DevCon->IASetInputLayout(vertLayout);

	AMD3D->d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	AMD3D->d3d11DevCon->DrawIndexed(numberOfIndicies, 0, 0);

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
	CubesTexSamplerState->Release();
	for (int i = 0; i < numberOfTextures; i++)
		textures[i]->Release();
	delete[] textures;
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
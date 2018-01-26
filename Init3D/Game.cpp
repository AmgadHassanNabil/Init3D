#include "Game.h"

Game* Game::instance;

Game::Game()
{
	ZeroMemory(this, sizeof(this));
}

bool Game::initialize(UINT width, UINT height)
{
	this->width = width;
	this->height = height;


	effect = new Effect(L"TexturedEffect.fx");
	HRESULT hr = model.loadFromFile("D:\\Graphics\\302.fbx", AMD3D->d3d11Device, effect, sizeof(cbPerObject));
	if (FAILED(hr)) return false;
	hr = cube.createTexturedCube(effect, AMD3D->d3d11Device, L"braynzar.jpg", sizeof(cbPerObject));
	if (FAILED(hr)) return false;
	hr = cube2.createTexturedCube(effect, AMD3D->d3d11Device, L"braynzar.jpg", sizeof(cbPerObject));
	if (FAILED(hr)) return false;

	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = AMD3D->d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);

	cbPerFrame.light.lightDirection = XMFLOAT3(0.3f, 0.5f, 0.2f);
	cbPerFrame.light.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cbPerFrame.light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	cubeWorld = XMMatrixScaling(20, 20, 20) * XMMatrixTranslation(0, 0, 400);
	cube2World = XMMatrixScaling(20, 20, 20) * XMMatrixTranslation(0, 0, -400);

	return true;
}

float yRotate = -XM_2PI;

void Game::update(const double& time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[])
{
	World = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(yRotate);

	if (INPUT_DOWN(mouseCurrState.rgbButtons[0]))
		yRotate += (XM_2PI) * time;
	if (INPUT_DOWN(mouseCurrState.rgbButtons[1]))
		yRotate -= (XM_2PI) * time;

	if (INPUT_DOWN(currKeyboardState[DIK_C]))
		camera.moveAcceleration = 60.0f;
	else
		camera.moveAcceleration = 30.0f;

	camera.update(time, mouseCurrState, currKeyboardState);
}

void Game::draw(const int& fps)
{
	AMD3D->d3d11DevCon->ClearRenderTargetView(AMD3D->renderTargetView, bgColor);
	AMD3D->d3d11DevCon->ClearDepthStencilView(AMD3D->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	camera.getView(camView);
	
#ifdef _WIN64
	WVP = XMMatrixMultiply(World, XMMatrixMultiply(camView, camProjection));
#else
	WVP = World * camView * camProjection
#endif
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(World);
	

	AMD3D->d3d11DevCon->UpdateSubresource(cbPerFrameBuffer, 0, NULL, &cbPerFrame, 0, 0);
	AMD3D->d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);

	model.draw(AMD3D->d3d11DevCon, &cbPerObj);

#ifdef _WIN64
	WVP = XMMatrixMultiply(cubeWorld, XMMatrixMultiply(camView, camProjection));
#else
	WVP = World * camView * camProjection
#endif
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(cubeWorld);

	cube.draw(AMD3D->d3d11DevCon, &cbPerObj);

#ifdef _WIN64
	WVP = XMMatrixMultiply(cube2World, XMMatrixMultiply(camView, camProjection));
#else
	WVP = World * camView * camProjection
#endif
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(cube2World);
	cube2.draw(AMD3D->d3d11DevCon, &cbPerObj);

	AMD3D->SwapChain->Present(0, 0);
}

void Game::release()
{
	cbPerFrameBuffer->Release();
	cube.release();
	cube2.release();
	model.release();
	effect->release();
	delete effect;
	delete this;
}

Game::~Game()
{
}
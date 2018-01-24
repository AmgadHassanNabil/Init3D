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
	HRESULT hr = model.loadFromFile("D:\\Graphics\\302.fbx", effect, sizeof(cbPerObject));
	if (FAILED(hr)) return false;

	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f);
	

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
		camera.moveAcceleration = 30.0f;
	else
		camera.moveAcceleration = 15.0f;

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
	cbPerObj.lightDirection = XMFLOAT3(0.3f, 0.5f, 0.2f);

	model.draw(&cbPerObj);
	AMD3D->SwapChain->Present(0, 0);
}

void Game::release()
{
	model.release();
	effect->release();
	delete effect;
	delete this;
}

Game::~Game()
{
}
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


	effect = new TexturedEffect();
	HRESULT hr = ship.initialize("D:\\Graphics\\302.fbx", effect);
	if (FAILED(hr)) return false;
	

	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 10000.0f);


	effect->frameConfigurations.light.lightDirection = XMFLOAT3(0.3f, 0.5f, 0.2f);
	effect->frameConfigurations.light.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	effect->frameConfigurations.light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	for (int x = 0; x < 20; x++)
		for (int y = 0; y < 20; y++)
		{
			hr = cube[x][y].createTexturedCube(effect, AMD3D->d3d11Device, L"braynzar.jpg", sizeof(cbPerObject));
			if (FAILED(hr)) return false;
			cubeWorld[x][y] = XMMatrixScaling(10, 10, 10) * XMMatrixTranslation((x * 300) - 1500, 0, (y * 300) - 1500);
		}
	return true;
}

int g = 0;

bool flag = true;
void Game::update(const double& time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[])
{

	if (INPUT_DOWN(currKeyboardState[DIK_B]))
	{
		int x = 0;
	}
	
	
	if (INPUT_DOWN(currKeyboardState[DIK_P]))
	{
		if (flag)
		{
			if (g == 0)
				AMD3D->enableAlphaBlending();
			else if (g == 1)
				AMD3D->enableAdditiveBlending();
			else if (g == 2)
			{
				g = -1;
				AMD3D->enableDefaultBlending();
			}
			g++;
		}
		flag = false;
	}
	else
		flag = true;

	ship.update(time, mouseCurrState, currKeyboardState);
	camera.update(time, mouseCurrState, ship.position, ship.direction, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void Game::draw(const int& fps)
{
	AMD3D->d3d11DevCon->ClearRenderTargetView(AMD3D->renderTargetView, bgColor);
	AMD3D->d3d11DevCon->ClearDepthStencilView(AMD3D->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	camera.getView(camView);

#ifdef _WIN64
	XMMATRIX VP = XMMatrixMultiply(camView, camProjection);
#else
	XMMATRIX VP = camView * camProjection;
#endif
	
	effect

	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{

#ifdef _WIN64
			WVP = XMMatrixMultiply(cubeWorld[x][y], VP);
#else
			WVP = World * camView * camProjection
#endif
			cbPerObj.WVP = XMMatrixTranspose(WVP);
			cbPerObj.World = XMMatrixTranspose(cubeWorld[x][y]);

			cube[x][y].draw(AMD3D->d3d11DevCon, &cbPerObj);
		}

	}

	ship.draw(VP);

	AMD3D->SwapChain->Present(0, 0);
}

void Game::release()
{
	cbPerFrameBuffer->Release();
	for (int x = 0; x < 20; x++)
		for (int y = 0; y < 20; y++)
			cube[x][y].release();
	ship.release();
	effect->release();
	delete effect;
	delete this;
}

Game::~Game()
{
}
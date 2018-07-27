#include "Game.h"

Game* Game::instance;

#define NUMBER_BOXES 20
#define BOX_SIZE 50

Game::Game()
{
	ZeroMemory(this, sizeof(this));
}

SpriteBatch spriteBatch;
SpriteEffect spriteEffect;

inline XMFLOAT3 boxPosition(int x, int y)
{
	return XMFLOAT3((x * ((NUMBER_BOXES * (BOX_SIZE * 7.5f)) / 5)) - (NUMBER_BOXES * 100), 0, (y * ((NUMBER_BOXES * (BOX_SIZE * 7.5f)) / 5)) - (NUMBER_BOXES * 100));
}
bool Game::initialize(UINT width, UINT height)
{
	this->width = width;
	this->height = height;

	HRESULT hr;
	srand(time(NULL));

	hr = ParticleEffect::createParticleEffect(AMD3D->d3d11Device, particleEffect);
	if (FAILED(hr)) return false;
	hr =  TexturedEffect::createTexturedEffect(AMD3D->d3d11Device, effect);
	if (FAILED(hr)) return false;
	//hr = SpriteEffect::createSpriteEffect(AMD3D->d3d11Device, spriteEffect);
	//if (FAILED(hr)) return false;

	hr = ship.initialize("D:\\Graphics\\302.fbx", "D:\\Graphics\\missle.fbx", L"fire.png", &effect, &particleEffect);
	if (FAILED(hr)) return false;


	//spriteBatch.initialize(AMD3D->d3d11Device, &spriteEffect, width, height);

	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 10000.0f);


	effect.frameConfigurations.light.lightDirection = XMFLOAT3(0.3f, 0.5f, 0.2f);
	effect.frameConfigurations.light.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	effect.frameConfigurations.light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	for (int x = 0; x < NUMBER_BOXES; x++)
		for (int y = 0; y < NUMBER_BOXES; y++)
		{
			hr = cube[x][y].createTexturedCube(&effect, AMD3D->d3d11Device, L"braynzar.jpg");
			if (FAILED(hr)) return false;
			XMFLOAT3 boxpos = boxPosition(x, y);
			cubeWorld[x][y] = XMMatrixScaling(BOX_SIZE, BOX_SIZE, BOX_SIZE) * XMMatrixTranslation(boxpos.x, boxpos.y, boxpos.z);
		}

	return true;
}

int g = 0;

double totalTime = 0;
bool flag = true, thrust = false;
float x = 0, y = 0, z = 0;
void Game::update(const double& time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[])
{
	
	if (INPUT_DOWN(currKeyboardState[DIK_X]))
	{
		thrust = true;
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

	if (INPUT_DOWN(currKeyboardState[DIK_I]))
		y += 0.1f;
	if (INPUT_DOWN(currKeyboardState[DIK_K]))
		y -= 0.1f;
	if (INPUT_DOWN(currKeyboardState[DIK_J]))
		x += 0.1f;
	if (INPUT_DOWN(currKeyboardState[DIK_L]))
		x -= 0.1f;
	if (INPUT_DOWN(currKeyboardState[DIK_Y]))
		z += 0.1f;
	if (INPUT_DOWN(currKeyboardState[DIK_U]))
		z -= 0.1f;
	camera.getView(camView);

	if (INPUT_DOWN(currKeyboardState[DIK_B]))
	{
		int x = 0;
	}

	XMFLOAT3 startPositionF, directionF, hitPoint;
	XMVECTOR startPosition, direction;
	float distance = 99999;
	int x, y;
	bool rayCollidedThisFrame = false;

	//ray = Ray(XMFLOAT3(0 + y, 25, -75 + x), XMFLOAT3(-1, 0, 0));


	//XMFLOAT3 min = XMFLOAT3(-100, 0, -100);
	//XMFLOAT3 max = XMFLOAT3(-50, 50, -50);
	

	//if (ray.intersect(AABB))
	//	selectedBox = 0;
	//else
	//	selectedBox = -1;

	//ray.pick(mouseCurrState.lX + (width/ 2), mouseCurrState.lY + (width / 2), camProjection, camView, XMMatrixIdentity(), width, height);
	camera.getPosition(startPosition);
	XMVECTOR lookAt;
	camera.getLookAt(lookAt);
	XMVECTOR delta = lookAt - startPosition;
	direction = XMVector3Normalize(delta);
	XMStoreFloat3(&startPositionF, startPosition);
	XMStoreFloat3(&directionF, direction);
	ray = Ray(startPositionF, directionF);

	for ( x = 0; x < NUMBER_BOXES; x++)
		for ( y = 0; y < NUMBER_BOXES; y++)
		{
			XMFLOAT3 min = boxPosition(x, y);
			min.x -= BOX_SIZE * 5;
			min.y -= BOX_SIZE * 5;
			min.z -= BOX_SIZE * 5;
			XMFLOAT3 max = XMFLOAT3(min.x, min.y, min.z);
			max.x += BOX_SIZE * 6;
			max.y += BOX_SIZE * 6;
			max.z += BOX_SIZE * 6;
			XMFLOAT3 AABB[] = { min, max };
			float currDistance;

			if (ray.intersect(AABB, currDistance))
			{
				selectedBox = x + y * NUMBER_BOXES;
				distance = currDistance;
				rayCollidedThisFrame = true;
			}
		}
	if (!rayCollidedThisFrame)
		selectedBox = -1;
	ship.update(time, mouseCurrState, currKeyboardState);
	camera.update(time, mouseCurrState, ship.position, ship.direction, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)); 
}

void Game::draw(const int& fps)
{
	AMD3D->d3d11DevCon->ClearRenderTargetView(AMD3D->renderTargetView, bgColor);
	AMD3D->d3d11DevCon->ClearDepthStencilView(AMD3D->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	

#ifdef _WIN64
	XMMATRIX VP = XMMatrixMultiply(camView, camProjection);
#else
	XMMATRIX VP = camView * camProjection;
#endif

	effect.apply();

	for (int x = 0; x < NUMBER_BOXES; x++)
	{
		for (int y = 0; y < NUMBER_BOXES; y++)
		{

			WVP = XMMatrixMultiply(cubeWorld[x][y], VP);
			effect.cbPerObj.WVP = XMMatrixTranspose(WVP);
			effect.cbPerObj.World = XMMatrixTranspose(cubeWorld[x][y]);
			effect.cbPerObj.isSelected = false;
			if (x + y * NUMBER_BOXES == selectedBox)
				effect.cbPerObj.isSelected = true;
			cube[x][y].draw(AMD3D->d3d11DevCon);
		}

	}

	XMVECTOR camPos, camUp;
	camera.getPosition(camPos);
	camera.getUp(camUp);
	XMFLOAT4 fCamPos;
	XMFLOAT3 fCamUp;
	XMStoreFloat4(&fCamPos, camPos);
	XMStoreFloat3(&fCamUp, camUp);
	effect.cbPerObj.isSelected = false;
	ship.draw(VP, fCamPos, fCamUp);	

	//spriteBatch.draw(AMD3D->d3d11DevCon);

	AMD3D->SwapChain->Present(0, 0);
}

void Game::release()
{
	
	for (int x = 0; x < NUMBER_BOXES; x++)
		for (int y = 0; y < NUMBER_BOXES; y++)
			cube[x][y].release();
	ship.release();
	effect.release();
	particleEffect.release();
	delete this;
}

Game::~Game()
{
}
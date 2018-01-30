#include "Game.h"

Game* Game::instance;

Game::Game()
{
	ZeroMemory(this, sizeof(this));
}

float _302Accelaration = 0.001f;
XMVECTOR position = XMVectorSet(0, 0, 0, 0);
XMMATRIX _302RotationMatrix = XMMatrixIdentity();
XMVECTOR up, direction, right, velocity;
const float RotationRate = 1.5f;
const float Mass = 1.0f;
const float ThrustForce = 24000.0f;
const float DragFactor = 0.97f;

bool Game::initialize(UINT width, UINT height)
{
	this->width = width;
	this->height = height;


	effect = new Effect(L"TexturedEffect.fx");
	HRESULT hr = model.loadFromFile("D:\\Graphics\\302.fbx", AMD3D->d3d11Device, effect, sizeof(cbPerObject));
	if (FAILED(hr)) return false;
	

	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 10000.0f);

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

	for (int x = 0; x < 10; x++)
		for (int y = 0; y < 10; y++)
		{
			hr = cube[x][y].createTexturedCube(effect, AMD3D->d3d11Device, L"braynzar.jpg", sizeof(cbPerObject));
			if (FAILED(hr)) return false;
			cubeWorld[x][y] = XMMatrixScaling(10, 10, 10) * XMMatrixTranslation((x * 150) - 750, 0, (y * 150) - 750);
		}

	position = XMVectorZero();
	direction = XMVectorSet(0, 0, 1, 1);
	up = XMVectorSet(0, 1, 0, 1);
	right = XMVectorSet(1, 0, 0, 1);
	velocity = XMVectorZero();
	return true;
}
bool motionStarted = false;

float thrustAmount = 0;
void Game::update(const double& time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[])
{
	XMFLOAT2 rotationAmount(0, 0);

	if (INPUT_DOWN(currKeyboardState[DIK_LEFT]))
		rotationAmount.x = -1.0f;
	if (INPUT_DOWN(currKeyboardState[DIK_RIGHT]))
		rotationAmount.x = 1.0f;
	if (INPUT_DOWN(currKeyboardState[DIK_UP]))
		rotationAmount.y = 1.0f;
	if (INPUT_DOWN(currKeyboardState[DIK_DOWN]))
		rotationAmount.y = -1.0f;


	if (INPUT_DOWN(currKeyboardState[DIK_RETURN]))
		motionStarted = true;

	float turningSpeed = time * 1.5f;
	rotationAmount.x *= turningSpeed;
	rotationAmount.y *= turningSpeed;

	if (XMVectorGetY(up) < 0)
		rotationAmount.x = -rotationAmount.x;

	XMMATRIX rotationMatrix =
		XMMatrixRotationAxis(right, rotationAmount.y) *
		XMMatrixRotationY(rotationAmount.x);

	direction = XMVector3TransformNormal (direction, rotationMatrix);
	up = XMVector3TransformNormal(up, rotationMatrix);
	direction = XMVector3Normalize(direction);
	up = XMVector3Normalize(up);

	right = XMVector3Cross(direction, up);

	up = XMVector3Cross(right, direction);

	if (INPUT_DOWN(currKeyboardState[DIK_SPACE]))
		thrustAmount += 0.005;
	else if(motionStarted)
		thrustAmount -= 0.0004f;

	if (motionStarted)
		thrustAmount = clamp(0.01, 1, thrustAmount);

	XMVECTOR force = direction * thrustAmount * ThrustForce;

	XMVECTOR acceleration = force / Mass;
	velocity += acceleration * time;

	velocity *= DragFactor;
	XMFLOAT3 vel;
	XMStoreFloat3(&vel, velocity);
	position += (-velocity) * time;

	XMMATRIX transform = TRANSFORMATION_MATRIX(direction, up, right, position); 

	World = XMMatrixScaling(0.05f, 0.05f, 0.05f) * transform; 

	XMFLOAT3 pos, dir;
	XMStoreFloat3(&pos, position);
	XMStoreFloat3(&dir, direction);


	camera.update(time, position, direction, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
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

	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{

#ifdef _WIN64
			WVP = XMMatrixMultiply(cubeWorld[x][y], XMMatrixMultiply(camView, camProjection));
#else
			WVP = World * camView * camProjection
#endif
			cbPerObj.WVP = XMMatrixTranspose(WVP);
			cbPerObj.World = XMMatrixTranspose(cubeWorld[x][y]);

			cube[x][y].draw(AMD3D->d3d11DevCon, &cbPerObj);
		}

	}


	AMD3D->SwapChain->Present(0, 0);
}

void Game::release()
{
	cbPerFrameBuffer->Release();
	for (int x = 0; x < 10; x++)
		for (int y = 0; y < 10; y++)
			cube[x][y].release();
	model.release();
	effect->release();
	delete effect;
	delete this;
}

Game::~Game()
{
}
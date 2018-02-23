#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <D3Dcompiler.h>
#include <dinput.h>

#include "Direct3D.h"
#include "FirstPersonCamera.h"
#include "Input.h"
#include "Model.h"
#include "TexturedEffect.h"
#include "ThirdPersonCamera.h"
#include "Ship.h"

using namespace DirectX;


class Game
{
private:

	const FLOAT bgColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	UINT width, height;

	Model cube[20][20];
	TexturedEffect* effect;

	XMMATRIX WVP;
	XMMATRIX camView;
	XMMATRIX camProjection;
	XMMATRIX cubeWorld[20][20];

	ThirdPersonCamera camera;

	Ship ship;

	

	Game();
	Game(const Game&);
	Game& operator=(const Game&);
	~Game();

	static Game* instance;

public:
	bool initialize(UINT, UINT);
	void update(const double& time, DIMOUSESTATE, BYTE[]);
	void draw(const int&);
	void release();

	inline static Game* getInstance() 
	{
		if (instance == nullptr)
			instance = new Game();
		return instance;
	}
};
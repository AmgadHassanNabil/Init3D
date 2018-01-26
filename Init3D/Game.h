#pragma once

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <D3Dcompiler.h>
#include <dinput.h>

#include "Direct3D.h"
#include "Camera.h"
#include "Vertices.h"
#include "WICTextureLoader.h"
#include "Input.h"
#include "FBXImporter.h"
#include "Model.h"
#include "Effect.h"

using namespace DirectX;


class Game
{
private:

	const FLOAT bgColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	UINT width, height;


	Model model, cube, cube2;
	Effect* effect;

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;
	XMMATRIX cubeWorld;
	XMMATRIX cube2World;

	Camera camera;

	struct Light
	{
		Light()
		{
			ZeroMemory(this, sizeof(Light));
		}
		XMFLOAT3 lightDirection;
		float pad;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
	};

	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;
	};

	struct cbPerFrame
	{
		Light light;
	};

	ID3D11Buffer* cbPerFrameBuffer;
	cbPerFrame cbPerFrame;
	cbPerObject cbPerObj;

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
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

using namespace DirectX;


class Game
{
private:

	const FLOAT bgColor[4] = { 0.0f, 0.0f, 0.7f, 1.0f };
	UINT width, height;

	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11Buffer* cubeVertBuffer;
	ID3D11Buffer* cubeIndexBuffer;
	ID3D11InputLayout* vertLayout;
	ID3DBlob* VS_Buffer = NULL;
	ID3DBlob* PS_Buffer = NULL;
	ID3DBlob* errorBlob = NULL;
	ID3D11Buffer* cbPerObjectBuffer;

	ID3D11ShaderResourceView** textures;
	ID3D11SamplerState* CubesTexSamplerState;

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;

	Camera camera;

	DWORD numberOfVerticies, numberOfIndicies, numberOfTextures;

	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;
		XMFLOAT3 lightDirection;
		float padding;
	};

	cbPerObject cbPerObj;

	Game();
	Game(const Game&);
	Game& operator=(const Game&);
	~Game();

	

	static Game* instance;

	bool loadModel();

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
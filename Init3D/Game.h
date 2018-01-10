#pragma once

#include "Headers.h"
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

	ID3D11ShaderResourceView* CubesTexture;
	ID3D11SamplerState* CubesTexSamplerState;

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	DWORD numberOfVerticies, numberOfIndicies;

	struct cbPerObject
	{
		XMMATRIX  WVP;
		//XMMATRIX World;
		//XMFLOAT3 lightDirection;
		//float padding;
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

	static Game* getInstance();
};
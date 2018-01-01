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
	ID3D11Buffer* quadVertBuffer;
	ID3D11Buffer* quadIndexBuffer;
	ID3D11InputLayout* vertLayout;
	ID3DBlob* VS_Buffer = NULL;
	ID3DBlob* PS_Buffer = NULL;
	ID3DBlob* errorBlob = NULL;
	ID3D11Buffer* cbPerObjectBuffer;

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	struct cbPerObject
	{
		XMMATRIX  WVP;
	};

	cbPerObject cbPerObj;

	Game();
	Game(const Game&);
	Game& operator=(const Game&);
	~Game();

	static Game* instance;

public:
	bool initialize(UINT, UINT);
	void update();
	void draw();
	void release();

	static Game* getInstance();
};
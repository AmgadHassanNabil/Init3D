#pragma once
#include <d3d11.h>
#include "Vertices.h"
#include "SpriteEffect.h"

class SpriteBatch
{
	int screenWidth, screenHeight;

	ID3D11Buffer* vertBuffer;
	ID3D11Buffer* indexBuffer;
	SpriteEffect* spriteEffect;
	ID3D11SamplerState* textureSamplerState;
public:
	SpriteBatch();
	~SpriteBatch();

	void initialize(ID3D11Device* device, SpriteEffect* effect, int width, int height);
	void release();

	void moveTexture(int, int x, int y);
	void viewTexture(int, bool);
	int addTexture(ID3D11ShaderResourceView* texture, int initX, int initY);
	void draw(ID3D11DeviceContext* d3d11DevCon);

};


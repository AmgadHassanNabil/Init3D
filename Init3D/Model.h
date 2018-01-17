#pragma once
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "FBXImporter.h"
#include "Defines.h"
#include "Direct3D.h"
#include "Vertices.h"
#include "Effect.h"

class Model
{
	ID3D11Buffer* vertBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11InputLayout* vertLayout;
	ID3D11ShaderResourceView** textures;
	DWORD numberOfVerticies, numberOfIndicies, numberOfTextures;
	ID3D11SamplerState* textureSamplerState;
	Effect *effect;
	ID3D11Buffer* cbPerObjectBuffer;

	Model();
	Model(const Model&);
	Model& operator=(const Model&);
public:
	~Model();
	Model(const char*, Effect *effect, UINT sizeOfConstantBuffer);
	void draw(const void* constantBufferData);
	void release();
};


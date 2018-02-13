#pragma once
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <fbxsdk.h>

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

	HRESULT createVertexAndIndexBuffers(ID3D11Device* device, VertexPositionNormalTexture* verticies, int numberOfVerticies, DWORD* indicies, int numberOfIndicies);

	Model(const Model&);
	Model& operator=(const Model&);
public:
	~Model();
	Model();

	HRESULT loadFromFile(const char* fileName, ID3D11Device* device, Effect *effect, const UINT sizeOfConstantBuffer);
	HRESULT createTexturedCube(Effect * effect, ID3D11Device* device, const wchar_t* textureName, const UINT sizeOfConstantBuffer);
	void draw(ID3D11DeviceContext* d3d11DevCon, const void* constantBufferData);
	void release();
};


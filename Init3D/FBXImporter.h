#pragma once
//#include"Headers.h"
#include <windows.h>
#include <fbxsdk.h>
#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>
#include "WICTextureLoader.h"
#include "Vertices.h"

using namespace DirectX;

class FBXImporter
{
	FbxManager* g_pFbxSdkManager;
	static FBXImporter* instance;

	inline HRESULT loadVerteciesPositions(FbxMesh * pMesh, FbxVector4 ** verticiesPositions, DWORD & numberOfVerticies);
	inline HRESULT loadIndicies(FbxMesh* pMesh, DWORD** indicies, DWORD &numberOfIndicies);
	inline HRESULT loadNormals(FbxMesh* pMesh, FbxVector4** normals);
	inline HRESULT loadUVs(FbxMesh* pMesh, FbxVector2** uvs);
	inline HRESULT loadTextures(ID3D11Device* device, FbxScene* pScene, DWORD& textureCount, ID3D11ShaderResourceView** &modelTextures);

public:
	FBXImporter();
	~FBXImporter();

	static FBXImporter* getInstance();

	HRESULT parseFBX(ID3D11Device* device, const char * fileName,
		FbxVector4** verticiesPositions, DWORD &numberOfVerticies,
		DWORD** indicies, DWORD &numberOfIndicies,
		FbxVector4** normals, FbxVector2** uvs,
		ID3D11ShaderResourceView** &outModelTextures, DWORD &textureCount);

	HRESULT parseFBX(ID3D11Device* device, const char * fileName,
		VertexPositionNormalTexture** modelVerticies,
		DWORD &numberOfVerticies,
		DWORD** indicies, DWORD &numberOfIndicies,
		ID3D11ShaderResourceView** &outModelTextures, DWORD &numberOfTextures);

	void release();
};


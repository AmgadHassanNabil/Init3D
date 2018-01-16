#pragma once
//#include"Headers.h"
#include <windows.h>
#include <fbxsdk.h>
#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>
#include "WICTextureLoader.h"
using namespace DirectX;

class FBXImporter
{
	FbxManager* g_pFbxSdkManager;
	static FBXImporter* instance;

	inline HRESULT loadVerteciesPositions(FbxMesh * pMesh, XMFLOAT3 ** verticiesPositions, DWORD & numberOfVerticies);
	inline HRESULT loadIndicies(FbxMesh* pMesh, DWORD** indicies, DWORD &numberOfIndicies);
	inline HRESULT loadNormals(FbxMesh* pMesh, XMFLOAT3** normals);
	inline HRESULT loadUVs(FbxMesh* pMesh, XMFLOAT2** uvs);
	inline HRESULT loadTextures(ID3D11Device* device, FbxScene* pScene, DWORD& textureCount, ID3D11ShaderResourceView** &modelTextures);

public:
	FBXImporter();
	~FBXImporter();

	static FBXImporter* getInstance();

	HRESULT parseFBX(ID3D11Device* device, const char * fileName,
		XMFLOAT3** verticiesPositions, DWORD &numberOfVerticies, 
		DWORD** indicies, DWORD &numberOfIndicies,
		XMFLOAT3** normals, XMFLOAT2** uvs,
		ID3D11ShaderResourceView** &outModelTextures, DWORD &numberOfTextures);

	void release();
};


#pragma once
#include"Headers.h"
#include<fbxsdk.h>
#include<vector>

class FBXImporter
{
	FbxManager* g_pFbxSdkManager;
	static FBXImporter* instance;

	inline HRESULT loadVerteciesPositions(FbxMesh * pMesh, XMFLOAT3 ** verticiesPositions, DWORD & numberOfVerticies);
	inline HRESULT loadIndicies(FbxMesh* pMesh, DWORD** indicies, DWORD &numberOfIndicies);
	inline HRESULT loadNormals(FbxMesh* pMesh, XMFLOAT3** normals);
	inline HRESULT loadUVs(FbxMesh* pMesh);
	inline HRESULT loadTextures(FbxMesh* pMesh);


public:
	FBXImporter();
	~FBXImporter();

	static FBXImporter* getInstance();

	HRESULT parseFBX(const char * fileName,
		XMFLOAT3** verticiesPositions, DWORD &numberOfVerticies, 
		DWORD** indicies, DWORD &numberOfIndicies,
		XMFLOAT3** normals);

	void release();
};


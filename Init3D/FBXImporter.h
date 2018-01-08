#pragma once
#include"Headers.h"
#include<fbxsdk.h>
#include<vector>

class FBXImporter
{
	static FbxManager* g_pFbxSdkManager;
	static FBXImporter* instance;


public:
	FBXImporter();
	~FBXImporter();

	static FBXImporter* getInstance();

	HRESULT parseFBX(const char * fileName, XMFLOAT3** verticiesPositions, DWORD &numberOfVerticies, DWORD** indicies, DWORD &numberOfIndicies);

	void release();
};


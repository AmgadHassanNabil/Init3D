#include "FBXImporter.h"

FbxManager* FBXImporter::g_pFbxSdkManager;
FBXImporter* FBXImporter::instance;

FBXImporter::FBXImporter()
{
	ZeroMemory(this, sizeof(this));	

	this->g_pFbxSdkManager = FbxManager::Create();
	FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
	g_pFbxSdkManager->SetIOSettings(pIOsettings);
}


FBXImporter::~FBXImporter()
{
}

FBXImporter * FBXImporter::getInstance()
{
	if (instance == nullptr)
		instance = new FBXImporter();
	return instance;
}

HRESULT FBXImporter::parseFBX(const char * fileName, XMFLOAT3** verticiesPositions, DWORD &numberOfVerticies, DWORD** indicies, DWORD &numberOfIndicies)
{
	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");
	bool bSuccess = pImporter->Initialize(fileName, -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess) return E_FAIL;

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return E_FAIL;

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = pFbxChildNode->GetMesh();

			numberOfVerticies = pMesh->GetControlPointsCount();
			*verticiesPositions = new XMFLOAT3[numberOfVerticies];

			for (DWORD j = 0; j < numberOfVerticies; j++)
			{
				FbxVector4 Vertex = pMesh->GetControlPointAt(j);
				(*verticiesPositions)[j] = XMFLOAT3((float)Vertex.mData[0], (float)Vertex.mData[1], (float)Vertex.mData[2]);
			}

			numberOfIndicies = pMesh->GetPolygonVertexCount();
			*indicies = (DWORD*)pMesh->GetPolygonVertices();
			int NumFaces = pMesh->GetPolygonCount();

		}

	}
	return S_OK;
}

void FBXImporter::release()
{
	delete this;
}


#include "FBXImporter.h"

#define ON_FAIL_RETURN_HRESULT(a)	if(FAILED(a)) return a


FBXImporter* FBXImporter::instance;

inline HRESULT FBXImporter::loadVerteciesPositions(FbxMesh* pMesh, FbxVector4** verticiesPositions, DWORD &numberOfVerticies)
{
	numberOfVerticies = pMesh->GetControlPointsCount();
	(*verticiesPositions) = pMesh->GetControlPoints();
	return S_OK;
}

inline HRESULT FBXImporter::loadIndicies(FbxMesh* pMesh, DWORD** indicies, DWORD &numberOfIndicies)
{
	numberOfIndicies = pMesh->GetPolygonVertexCount();
	*indicies = (DWORD*)pMesh->GetPolygonVertices();
	int NumFaces = pMesh->GetPolygonCount();
	return S_OK;
}

inline HRESULT FBXImporter::loadNormals(FbxMesh* pMesh, FbxVector4** normals)
{
	//pMesh->GenerateNormals(true, true);
	FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();
	if (lNormalElement)
	{
		if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			int numberOfNormals = pMesh->GetControlPointsCount();
			*normals = new FbxVector4[numberOfNormals];

			for (int lVertexIndex = 0; lVertexIndex < numberOfNormals; lVertexIndex++)
			{
				int lNormalIndex = 0;
				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
					lNormalIndex = lVertexIndex;

				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

				(*normals)[lNormalIndex] = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
			}
		}
		else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lIndexByPolygonVertex = 0;
			
			for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++)
			{
				int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
			
				for (int i = 0; i < lPolygonSize; i++)
				{
					int lNormalIndex = 0;
					
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
						lNormalIndex = lIndexByPolygonVertex;

					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);

					//(*normals)[lNormalIndex] = XMFLOAT3(lNormal[0], lNormal[1], lNormal[2], lNormal[3]);

					lIndexByPolygonVertex++;
				}
			}
				int x = 0;
				x++;
		}
	}

	return S_OK;
}

inline HRESULT FBXImporter::loadUVs(FbxMesh* pMesh, FbxVector2** uvs)
{
	FbxLayerElementUV *pFbxLayerElementUV = pMesh->GetLayer(0)->GetUVs();

	if (pFbxLayerElementUV == nullptr || pFbxLayerElementUV->GetMappingMode() == FbxLayerElementUV::eDirect)
		return E_FAIL;

	const int lPolyCount = pMesh->GetPolygonCount();

	*uvs = new FbxVector2[lPolyCount * 3];

	int vertexCounter = 0;
	for (int j = 0; j < lPolyCount; j++) {

		for (int k = 0; k < 3; k++) {

			int vertexIndex = pMesh->GetPolygonVertex(j, k);
			
			(*uvs)[vertexIndex] = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
			(*uvs)[vertexIndex].mData[1] = 1.0 - (*uvs)[vertexIndex].mData[1];
			vertexCounter++;
		}
	}
	
	return S_OK;
}

inline HRESULT FBXImporter::loadTextures(ID3D11Device* device, FbxScene* pScene, DWORD& textureCount, ID3D11ShaderResourceView** &outModelTextures)
{
	HRESULT hr;
	textureCount = pScene->GetTextureCount();
	outModelTextures = new ID3D11ShaderResourceView*[textureCount];
	for (int i = 0; i < textureCount; i++)
	{
		FbxTexture* texture = pScene->GetTexture(i);
		const FbxFileTexture *lFileTexture = (const FbxFileTexture*)(texture);
		const char* str = (const char*)lFileTexture->GetFileName();
		int fileNameLength = strlen(str);
		wchar_t wStr[100];
		MultiByteToWideChar(CP_ACP, 0, str, -1, &wStr[0], fileNameLength + 1);
		ID3D11ShaderResourceView* currTex = NULL;
		hr = CreateWICTextureFromFile(device, &wStr[0], nullptr, &currTex);

		//failed to Load file release previous files that are loaded
		if (FAILED(hr))
		{
			for (int j = 0; j < i; j++)
				outModelTextures[i]->Release();
			delete[] outModelTextures;
			break;
		}
		outModelTextures[i] = currTex;
	}
	
	return S_OK;
}

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

HRESULT FBXImporter::parseFBX(ID3D11Device* device, const char * fileName,
	FbxVector4** verticiesPositions, DWORD &numberOfVerticies,
	DWORD** indicies, DWORD &numberOfIndicies,
	FbxVector4** normals, FbxVector2** uvs,
	ID3D11ShaderResourceView** &outModelTextures, DWORD &textureCount)
{
	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");
	bool bSuccess = pImporter->Initialize(fileName, -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess) return E_FAIL;

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return ERROR_FILE_NOT_FOUND;

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
			bSuccess = pMesh->SplitPoints();
			if (!bSuccess) return E_FAIL;

			ON_FAIL_RETURN_HRESULT(loadVerteciesPositions(pMesh, verticiesPositions, numberOfVerticies));
			ON_FAIL_RETURN_HRESULT(loadIndicies(pMesh, indicies, numberOfIndicies));
			if (numberOfIndicies != numberOfVerticies) return E_FAIL;
			ON_FAIL_RETURN_HRESULT(loadNormals(pMesh, normals));
			ON_FAIL_RETURN_HRESULT(loadUVs(pMesh, uvs));
			ON_FAIL_RETURN_HRESULT(loadTextures(device, pFbxScene, textureCount, outModelTextures));
		}

	}
	return S_OK;
}

HRESULT FBXImporter::parseFBX(ID3D11Device * device, const char * fileName, VertexPositionNormalTexture ** modelVerticies, DWORD & numberOfVerticies, DWORD** indicies, DWORD & numberOfIndicies, ID3D11ShaderResourceView **& outModelTextures, DWORD & numberOfTextures)
{
	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");
	bool bSuccess = pImporter->Initialize(fileName, -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess) return E_FAIL;

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return ERROR_FILE_NOT_FOUND;

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
			bSuccess = pMesh->SplitPoints();
			if (!bSuccess) return E_FAIL;

			
			FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();
			if (lNormalElement)
			{
				if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					int numberOfVerticies = pMesh->GetControlPointsCount();
					*modelVerticies = new VertexPositionNormalTexture[numberOfVerticies];

					for (int lVertexIndex = 0; lVertexIndex < numberOfVerticies; lVertexIndex++)
					{
						FbxVector4 Vertex = pMesh->GetControlPointAt(lVertexIndex);
						(*modelVerticies)[lVertexIndex].pos = XMFLOAT3((float)Vertex.mData[0], (float)Vertex.mData[1], (float)Vertex.mData[2]);

						int lNormalIndex = 0;
						if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
							lNormalIndex = lVertexIndex;

						if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
							lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

						FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
						(*modelVerticies)[lNormalIndex].normal = XMFLOAT3(lNormal[0], lNormal[1], lNormal[2]);
					}
				}
			}
			
		}

	}
	return S_OK;
}

void FBXImporter::release()
{
	if (this->g_pFbxSdkManager) { this->g_pFbxSdkManager->Destroy(); this->g_pFbxSdkManager = NULL; }
	delete this;
}


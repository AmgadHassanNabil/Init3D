#include "FBXImporter.h"

FBXImporter* FBXImporter::instance;

inline HRESULT FBXImporter::loadVerteciesPositions(FbxMesh* pMesh, XMFLOAT3** verticiesPositions, DWORD &numberOfVerticies)
{
	numberOfVerticies = pMesh->GetControlPointsCount();
	*verticiesPositions = new XMFLOAT3[numberOfVerticies];

	for (DWORD j = 0; j < numberOfVerticies; j++)
	{
		FbxVector4 Vertex = pMesh->GetControlPointAt(j);
		(*verticiesPositions)[j] = XMFLOAT3((float)Vertex.mData[0], (float)Vertex.mData[1], (float)Vertex.mData[2]);
	}
	return S_OK;
}

inline HRESULT FBXImporter::loadIndicies(FbxMesh* pMesh, DWORD** indicies, DWORD &numberOfIndicies)
{
	numberOfIndicies = pMesh->GetPolygonVertexCount();
	*indicies = (DWORD*)pMesh->GetPolygonVertices();
	int NumFaces = pMesh->GetPolygonCount();
	return S_OK;
}

inline HRESULT FBXImporter::loadNormals(FbxMesh* pMesh, XMFLOAT3** normals)
{
	//pMesh->GenerateNormals(true, true);
	FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();
	if (lNormalElement)
	{
		if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			int numberOfNormals = pMesh->GetControlPointsCount();
			*normals = new XMFLOAT3[numberOfNormals];

			for (int lVertexIndex = 0; lVertexIndex < numberOfNormals; lVertexIndex++)
			{
				int lNormalIndex = 0;
				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
					lNormalIndex = lVertexIndex;

				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

				FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
				(*normals)[lNormalIndex] = XMFLOAT3(lNormal[0], lNormal[1], lNormal[2]);
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

inline HRESULT FBXImporter::loadUVs(FbxMesh* pMesh, XMFLOAT2** uvs)
{
	FbxStringList lUVSetNameList;
	pMesh->GetUVSetNames(lUVSetNameList);

	//iterating over all uv sets
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		//get lUVSetIndex-th uv set
		const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);

		if (!lUVElement)
			continue;

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return E_NOTIMPL;

		//index array, where holds the index referenced to the uv data
		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int lPolyCount = pMesh->GetPolygonCount();
		*uvs = new XMFLOAT2[lPolyCount * 3];

		if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					//get the index of the current vertex in control points array
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

					//the UV index depends on the reference mode
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

					FbxVector2 lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
					(*uvs)[lUVIndex] = XMFLOAT2(lUVValue.mData[0], lUVValue.mData[1]);
				}
			}
		}
		else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lPolyIndexCounter = 0;
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					if (lPolyIndexCounter < lIndexCount)
					{
						FbxVector2 lUVValue;

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

						//User TODO:
						//Print out the value of UV(lUVValue) or log it to a file

						lPolyIndexCounter++;
					}
				}
			}
		}
	}
	
	return E_NOTIMPL;
}

inline HRESULT FBXImporter::loadTextures(FbxScene* pScene, DWORD& textureCount, ID3D11ShaderResourceView** &outModelTextures)
{
	HRESULT hr;
	textureCount = pScene->GetTextureCount();
	outModelTextures = new ID3D11ShaderResourceView*[textureCount];
	for (int i = 0; i < textureCount; i++)
	{
		FbxTexture* texture = pScene->GetTexture(i);
		const FbxFileTexture *lFileTexture = (const FbxFileTexture*)(texture);
		const char* str = (const char*)lFileTexture->GetFileName();
		size_t fileNameLength = strlen(str);
		wchar_t wStr[100];
		MultiByteToWideChar(CP_ACP, 0, str, -1, &wStr[0], fileNameLength + 1);
		ID3D11ShaderResourceView* currTex = NULL;
		hr = CreateWICTextureFromFile(AMD3D->d3d11Device, &wStr[0], nullptr, &currTex);
		outModelTextures[i] = currTex;
		int x = 0;
	}
	
	return E_NOTIMPL;
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

HRESULT FBXImporter::parseFBX(const char * fileName,
	XMFLOAT3** verticiesPositions, DWORD &numberOfVerticies, 
	DWORD** indicies, DWORD &numberOfIndicies,
	XMFLOAT3** normals, XMFLOAT2** uvs,
	ID3D11ShaderResourceView** &outModelTextures, DWORD &textureCount)
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
			bSuccess = pMesh->SplitPoints();
			if (!bSuccess) return E_FAIL;

			loadVerteciesPositions(pMesh, verticiesPositions, numberOfVerticies);
			loadIndicies(pMesh, indicies, numberOfIndicies);
			loadNormals(pMesh, normals);
			loadUVs(pMesh, uvs);
			loadTextures(pFbxScene, textureCount, outModelTextures);
		}

	}
	return S_OK;
}

void FBXImporter::release()
{
	if (this->g_pFbxSdkManager) { this->g_pFbxSdkManager->Destroy(); this->g_pFbxSdkManager = NULL; }
	delete this;
}


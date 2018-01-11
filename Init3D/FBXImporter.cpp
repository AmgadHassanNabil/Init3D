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
	FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();
	if (lNormalElement)
	{
		//mapping mode is by control points. The mesh should be smooth and soft.
		//we can get normals by retrieving each control point
		if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			int numberOfNormals = pMesh->GetControlPointsCount();
			*normals = new XMFLOAT3[numberOfNormals];
			//Let's get normals of each vertex, since the mapping mode of normal element is by control point
			for (int lVertexIndex = 0; lVertexIndex < numberOfNormals; lVertexIndex++)
			{
				int lNormalIndex = 0;
				//reference mode is direct, the normal index is same as vertex index.
				//get normals by the index of control vertex
				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
					lNormalIndex = lVertexIndex;

				//reference mode is index-to-direct, get normals by the index-to-direct
				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

				//Got normals of each vertex.
				FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
				(*normals)[lNormalIndex] = XMFLOAT3(lNormal[0], lNormal[1], lNormal[2]);

				//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
				//. . .
			}//end for lVertexIndex
		}//end eByControlPoint
		 //mapping mode is by polygon-vertex.
		 //we can get normals by retrieving polygon-vertex.
		else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lIndexByPolygonVertex = 0;
			//Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
			for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++)
			{
				//get polygon size, you know how many vertices in current polygon.
				int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
				//retrieve each vertex of current polygon.
				for (int i = 0; i < lPolygonSize; i++)
				{
					int lNormalIndex = 0;
					//reference mode is direct, the normal index is same as lIndexByPolygonVertex.
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
						lNormalIndex = lIndexByPolygonVertex;

					//reference mode is index-to-direct, get normals by the index-to-direct
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

					//Got normals of each polygon-vertex.
					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);

					//(*normals)[lNormalIndex] = XMFLOAT3(lNormal[0], lNormal[1], lNormal[2], lNormal[3]);
					//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
					//. . .

					lIndexByPolygonVertex++;
				}//end for i //lPolygonSize
			}//end for lPolygonIndex //PolygonCount

		}//end eByPolygonVertex
	}//end if lNormalElement

	return E_NOTIMPL;
}

inline HRESULT FBXImporter::loadUVs(FbxMesh* pMesh)
{

	return E_NOTIMPL;
}

inline HRESULT FBXImporter::loadTextures(FbxMesh* pMesh)
{
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
	XMFLOAT3** normals)
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

			
			loadVerteciesPositions(pMesh, verticiesPositions, numberOfVerticies);
			loadIndicies(pMesh, indicies, numberOfIndicies);
			loadNormals(pMesh, normals);

		}

	}
	return S_OK;
}

void FBXImporter::release()
{
	if (this->g_pFbxSdkManager) { this->g_pFbxSdkManager->Destroy(); this->g_pFbxSdkManager = NULL; }
	delete this;
}


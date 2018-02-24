#include "Model.h"

Model::Model()
{
}

HRESULT Model::loadFromFile(const char * fileName, ID3D11Device* device, TexturedEffect * effect)
{
	this->effect = effect;
	FbxVector4* positions = NULL;
	DWORD* indiciesF = NULL;
	FbxVector4* normals = NULL;
	DWORD numberOfNormals;
	FbxVector2* uvs = NULL;

	HRESULT hr = FBXImporter::getInstance()->parseFBX(device, fileName,
		&positions, numberOfVerticies,
		&indiciesF, numberOfIndicies,
		&normals, &uvs, textures, numberOfTextures);
	if (FAILED(hr)) return hr;

	VertexPositionNormalTexture* modelVerticies = new VertexPositionNormalTexture[numberOfVerticies];

	for (DWORD i = 0; i < numberOfVerticies; i++)
	{
		FbxVector4 position = positions[i];
		FbxVector4 normal = normals[i];
		FbxVector2 uv = uvs[i];
		modelVerticies[i] = VertexPositionNormalTexture(XMFLOAT3(position.mData[0], position.mData[1], position.mData[2]),
			XMFLOAT2(uv.mData[0], uv.mData[1]),
			XMFLOAT3(normal.mData[0], normal.mData[1], normal.mData[2]));
	}
	delete[] normals;
	delete[] uvs;

	createVertexAndIndexBuffers(device, modelVerticies, numberOfVerticies, indiciesF, numberOfIndicies);
	delete[] modelVerticies;
	if (FAILED(hr)) return hr;


	

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = effect->getSizeOfPerObjectCB();
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	hr = device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
	if (FAILED(hr)) return hr;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 4;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampDesc, &textureSamplerState);

	return S_OK;
}

HRESULT Model::createTexturedCube(TexturedEffect * effect, ID3D11Device* device, const wchar_t* textureName)
{
	this->effect = effect;
	VertexPositionNormalTexture cubeVerticies[] =
	{
		// Front Face
		VertexPositionNormalTexture(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		VertexPositionNormalTexture(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		// Back Face
		VertexPositionNormalTexture(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
		VertexPositionNormalTexture(1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f, 1.0f),
		VertexPositionNormalTexture(1.0f,  1.0f, 1.0f, 0.0f, 0.0f,  1.0f,  1.0f, 1.0f),
		VertexPositionNormalTexture(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

		// Top Face
		VertexPositionNormalTexture(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		VertexPositionNormalTexture(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		VertexPositionNormalTexture(1.0f, 1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,  1.0f),
		VertexPositionNormalTexture(1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  1.0f, 1.0f, -1.0f),

		// Bottom Face
		VertexPositionNormalTexture(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		VertexPositionNormalTexture(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		// Left Face
		VertexPositionNormalTexture(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		VertexPositionNormalTexture(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		VertexPositionNormalTexture(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		VertexPositionNormalTexture(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		// Right Face
		VertexPositionNormalTexture(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		VertexPositionNormalTexture(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		VertexPositionNormalTexture(1.0f, -1.0f,  1.0f, 1.0f, 1.0f,1.0f, -1.0f,  1.0f),
	};

	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	numberOfVerticies = 24;
	numberOfIndicies = 36;

	textures = new ID3D11ShaderResourceView*[1];
	HRESULT hr = CreateWICTextureFromFile(device, &textureName[0], nullptr, &textures[0]);
	if (FAILED(hr)) return hr;
	hr = createVertexAndIndexBuffers(device, cubeVerticies, numberOfVerticies, indices, numberOfIndicies);
	if (FAILED(hr)) return hr;

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = effect->getSizeOfPerObjectCB();
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	hr = device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
	if (FAILED(hr)) return hr;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 4;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampDesc, &textureSamplerState);
	return S_OK;
}

void Model::draw(ID3D11DeviceContext* d3d11DevCon)
{
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &effect->cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	d3d11DevCon->PSSetShaderResources(0, 1, &textures[0]);
	d3d11DevCon->PSSetSamplers(0, 1, &textureSamplerState);

	d3d11DevCon->VSSetShader(effect->vs.VS, NULL, NULL);
	d3d11DevCon->PSSetShader(effect->ps.PS, NULL, NULL);

	UINT stride = sizeof(VertexPositionNormalTexture);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
	d3d11DevCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	d3d11DevCon->IASetInputLayout(effect->getInputLayout());

	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3d11DevCon->DrawIndexed(numberOfIndicies, 0, 0);
}

void Model::release()
{
	vertBuffer->Release();
	indexBuffer->Release();
	cbPerObjectBuffer->Release();
	textureSamplerState->Release();
	for (int i = 0; i < numberOfTextures; i++)
		textures[i]->Release();
	delete[] textures;
}


HRESULT Model::createVertexAndIndexBuffers(ID3D11Device * device, VertexPositionNormalTexture * verticies, int numberOfVerticies, DWORD * indicies, int numberOfIndicies)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionNormalTexture) * numberOfVerticies;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = verticies;
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertBuffer);

	if (FAILED(hr)) return hr;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * numberOfIndicies;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indicies;
	hr = device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);
	if (FAILED(hr)) return hr;

	return S_OK;
}

Model::~Model()
{
}
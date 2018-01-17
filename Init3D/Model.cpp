#include "Model.h"

Model::Model()
{
}

Model::Model(const char* fileName, Effect *effect, const UINT sizeOfConstantBuffer)
{
	this->effect = effect;
	XMFLOAT3* positions = NULL;
	DWORD* indiciesF = NULL;
	XMFLOAT3* normals = NULL;
	DWORD numberOfNormals;
	XMFLOAT2* uvs = NULL;

	HRESULT hr = FBXImporter::getInstance()->parseFBX(AMD3D->d3d11Device, fileName,
		&positions, numberOfVerticies,
		&indiciesF, numberOfIndicies,
		&normals, &uvs, textures, numberOfTextures);
	assert(SUCCEEDED(hr));

	VertexPositionNormalTexture* modelVerticies = new VertexPositionNormalTexture[numberOfVerticies];

	for (DWORD i = 0; i < numberOfVerticies; i++)
		modelVerticies[i] = VertexPositionNormalTexture(positions[i], uvs[i], normals[i]);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionNormalTexture) * numberOfVerticies;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = modelVerticies;
	hr = AMD3D->d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertBuffer);
	assert(SUCCEEDED(hr));

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * numberOfIndicies;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indiciesF;
	hr = AMD3D->d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);
	assert(SUCCEEDED(hr));

	hr = AMD3D->d3d11Device->CreateInputLayout(VertexPositionNormalTexture::layout, VertexPositionNormalTexture::numElements, effect->VS_Buffer->GetBufferPointer(),
		effect->VS_Buffer->GetBufferSize(), &vertLayout);
	assert(SUCCEEDED(hr));

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeOfConstantBuffer;
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	hr = AMD3D->d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
	assert(SUCCEEDED(hr));

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
	hr = AMD3D->d3d11Device->CreateSamplerState(&sampDesc, &textureSamplerState);

	delete[] modelVerticies;
	delete[] positions;
	delete[] normals;
	delete[] uvs;
}

void Model::draw(const void* constantBufferData)
{
	AMD3D->d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, constantBufferData, 0, 0);
	AMD3D->d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	AMD3D->d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	AMD3D->d3d11DevCon->PSSetShaderResources(0, 1, &textures[0]);
	AMD3D->d3d11DevCon->PSSetSamplers(0, 1, &textureSamplerState);

	AMD3D->d3d11DevCon->VSSetShader(effect->VS, NULL, NULL);
	AMD3D->d3d11DevCon->PSSetShader(effect->PS, NULL, NULL);

	UINT stride = sizeof(VertexPositionNormalTexture);
	UINT offset = 0;
	AMD3D->d3d11DevCon->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
	AMD3D->d3d11DevCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	AMD3D->d3d11DevCon->IASetInputLayout(vertLayout);

	AMD3D->d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	AMD3D->d3d11DevCon->DrawIndexed(numberOfIndicies, 0, 0);
}

void Model::release()
{
	vertBuffer->Release();
	indexBuffer->Release();
	vertLayout->Release();
	cbPerObjectBuffer->Release();
	textureSamplerState->Release();
	for (int i = 0; i < numberOfTextures; i++)
		textures[i]->Release();
	delete[] textures;
}


Model::~Model()
{
}
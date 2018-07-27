#include "SpriteBatch.h"



SpriteBatch::SpriteBatch()
{
}


SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::initialize(ID3D11Device * device, SpriteEffect* effect, int width, int height)
{
	this->spriteEffect = effect;
	this->screenWidth = width;
	this->screenHeight = height;

	VertexPositionTexture verticies[] =
	{
		VertexPositionTexture(-1, -1, 0, 0, 0),
		VertexPositionTexture(1, -1, 0, 1, 0),
		VertexPositionTexture(-1, 1, 0, 0, 1),
		VertexPositionTexture(1, 1, 0, 1, 1)
	};

	DWORD indices[] =
	{
		0,
		1,
		2,
		2,
		3,
		1
	};

	HRESULT hr;

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionTexture) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = verticies;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertBuffer);


	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	hr = device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);

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

}

void SpriteBatch::release()
{
	if(vertBuffer)
		vertBuffer->Release();
	if (indexBuffer)
		indexBuffer->Release();
	if (textureSamplerState)
		textureSamplerState->Release();
}

void SpriteBatch::moveTexture(int, int x, int y)
{
}

void SpriteBatch::viewTexture(int, bool)
{
}

int SpriteBatch::addTexture(ID3D11ShaderResourceView * texture, int initX, int initY)
{
	return 0;
}

void SpriteBatch::draw(ID3D11DeviceContext * d3d11DevCon)
{
	d3d11DevCon->VSSetShader(spriteEffect->vs.VS, NULL, NULL);
	d3d11DevCon->PSSetShader(spriteEffect->ps.PS, NULL, NULL);
	d3d11DevCon->GSSetShader(NULL, NULL, NULL);

	d3d11DevCon->PSSetSamplers(0, 1, &textureSamplerState);

	UINT stride = sizeof(VertexPositionTexture);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
	d3d11DevCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	d3d11DevCon->IASetInputLayout(spriteEffect->getInputLayout());

	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3d11DevCon->DrawIndexed(6, 0, 0);
}
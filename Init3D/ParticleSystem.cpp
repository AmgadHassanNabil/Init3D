#include "ParticleSystem.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <time.h>
#include <cwchar>

#define ONFAIL_RELEASE_RETURN(hr, a)	if (FAILED(hr)) { a->release(); return hr; }
#define SAFE_RELEASE(a)					if (a){ a->release(); delete a; a = nullptr; }
#define SAFE_RELEASE_N(a)				if (a){ a->Release(); a = nullptr; }
#define RAND(LO, HI)					LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)))

ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}


HRESULT ParticleSystem::init(ID3D11Device* device, ParticleEffect* fx, UINT maxParticles, const wchar_t* texturePath, float spread)
{

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
	HRESULT hr = device->CreateSamplerState(&sampDesc, &textureSamplerState);
	ONFAIL_RELEASE_RETURN(hr, this);

	this->mFX = fx;
	this->mMaxParticles = maxParticles;

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = fx->getSizeOfPerObjectCB();
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	hr = device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
	ONFAIL_RELEASE_RETURN(hr, this);

	//ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	//cbbd.Usage = D3D11_USAGE_DEFAULT;
	//cbbd.ByteWidth = sizeof(ParticleEffect::cbDynamics);
	//cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbbd.CPUAccessFlags = 0;
	//cbbd.MiscFlags = 0;

	//ParticleEffect::cbDynamics dynamics;
	//dynamics.direction = XMFLOAT3(0, 0, 40);
	//dynamics.ttl = 1;
	//dynamics.endSize = XMFLOAT2(0.0f, 0.0f);

	//D3D11_SUBRESOURCE_DATA dynamicsData;
	//dynamicsData.pSysMem = &dynamics;

	//hr = device->CreateBuffer(&cbbd, &dynamicsData, &cbDynamicsBuffer);
	//ONFAIL_RELEASE_RETURN(hr, this);

	buildVB(device, spread);

	mFX->objectConstantBuffer.billWidth = 2;
	mFX->objectConstantBuffer.billHeight = 2;
	mFX->objectConstantBuffer.direction = XMFLOAT3(0, 0, 40);
	mFX->objectConstantBuffer.ttl = 1;
	mFX->objectConstantBuffer.endSize = XMFLOAT2(0.0f, 0.0f);
	size_t pathLength = std::wcslen(texturePath);

	if(texturePath[pathLength - 3] == 'd' && texturePath[pathLength - 2] == 'd' && texturePath[pathLength - 1] == 's')
		hr = CreateDDSTextureFromFile(device, texturePath, nullptr, &particleTexture);
	else
		hr = CreateWICTextureFromFile(device, texturePath, nullptr, &particleTexture);

	if (FAILED(hr))
		ONFAIL_RELEASE_RETURN(hr, this);
	return S_OK;
}


void ParticleSystem::update(float dt, const XMFLOAT3& direction, const XMFLOAT3& emitPosition)
{
	this->mEmitPosW = emitPosition;
	this->mEmitDirW = direction;
	mFX->objectConstantBuffer.direction = this->mEmitDirW;
	mFX->objectConstantBuffer.emitPosition = this->mEmitPosW;
	mAge += dt;
}

void ParticleSystem::draw(ID3D11DeviceContext* dc, const XMMATRIX& VP, const XMFLOAT4& camPos, const XMFLOAT3& camUp)
{
	mFX->objectConstantBuffer.WVP = XMMatrixTranspose(VP);
	mFX->objectConstantBuffer.camPos = camPos;
	mFX->objectConstantBuffer.camUp = camUp;
	mFX->objectConstantBuffer.timeSinceInception = mAge;
	dc->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &mFX->objectConstantBuffer, 0, 0);

	dc->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	//dc->VSSetConstantBuffers(1, 1, &cbDynamicsBuffer);
	dc->GSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	dc->PSSetShaderResources(0, 1, &particleTexture);
	dc->PSSetSamplers(0, 1, &textureSamplerState);

	dc->VSSetShader(mFX->drawVS->VS, NULL, NULL);
	dc->PSSetShader(mFX->drawPS->PS, NULL, NULL);
	dc->GSSetShader(mFX->drawGS->GS, NULL, NULL);

	UINT strides[2] = { sizeof(ParticleVertex), sizeof(InstanceData) };
	UINT offsets[2] = { 0, 0 };

	// Store the vertex and instance buffers into an array
	// The leaves will use the same instance buffer as the trees, because we need each leaf
	// to go to a certain tree
	ID3D11Buffer* vertBillInstBuffers[2] = { vertexBuffer,instanceBuffer };

	//Set the models vertex and isntance buffer using the arrays created above
	dc->IASetVertexBuffers(0, 2, vertBillInstBuffers, strides, offsets);

	dc->IASetInputLayout(mFX->getInputLayout());

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	dc->DrawInstanced(1, mMaxParticles, 0, 0);
}

HRESULT ParticleSystem::buildVB(ID3D11Device* device, float spread)
{

	InstanceData* particleInstances = new InstanceData[mMaxParticles];
	int high = spread;
	int low = -spread;
	srand(time(NULL));
	for (int i = 0; i < mMaxParticles; i++)
	{
		float randX = rand() % (high - low + 1) + low;
		float randY = rand() % (high - low + 1) + low;
		float randZ = rand() % (high - low + 1) + low;
		particleInstances[i].pos = XMFLOAT3(randX, randY, randZ);

		randX = ((float)rand() / (RAND_MAX)) + 0.01f;
		randY = ((float)rand() / (RAND_MAX)) + 0.01f;
		randZ = ((float)rand() / (RAND_MAX)) + 0.01f;
		float randW = ((float)rand() / (RAND_MAX)) + 0.01f;
		particleInstances[i].random = XMFLOAT4(randX, randY, randZ, randW);
	}								 

	D3D11_BUFFER_DESC instBuffDesc;
	// Here we create the billboards instance buffer
	ZeroMemory(&instBuffDesc, sizeof(instBuffDesc));

	instBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	instBuffDesc.ByteWidth = sizeof(InstanceData) * mMaxParticles;
	instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instBuffDesc.CPUAccessFlags = 0;
	instBuffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ginitInistanceData;
	ginitInistanceData.pSysMem = particleInstances;
	HRESULT hr = device->CreateBuffer(&instBuffDesc, &ginitInistanceData, &instanceBuffer);
	ONFAIL_RELEASE_RETURN(hr, this);

	delete particleInstances;

	// Create the vertex buffer we will send to the shaders for the billboard data. We are going to use
	// the instancing technique for the billboards, and our billboard geometry shader only requires a single
	// point to be an input, so all we need to do is create a buffer that stores a single point!
	D3D11_BUFFER_DESC billboardBufferDesc;
	ZeroMemory(&billboardBufferDesc, sizeof(billboardBufferDesc));

	billboardBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	billboardBufferDesc.ByteWidth = sizeof(ParticleVertex);
	billboardBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	billboardBufferDesc.CPUAccessFlags = 0;
	billboardBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ginitVertexData;

	// Create a single vertex at the point 0,0,0. None of the other members will be used for billboarding
	ParticleVertex gv;
	gv.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	gv.textureCoords = XMFLOAT2(0.0f, 0.0f);

	ginitVertexData.pSysMem = &gv;
	hr = device->CreateBuffer(&billboardBufferDesc, &ginitVertexData, &vertexBuffer);
	ONFAIL_RELEASE_RETURN(hr, this);

	return S_OK;
}

void ParticleSystem::release()
{
	SAFE_RELEASE_N(vertexBuffer);
	SAFE_RELEASE_N(instanceBuffer); 
	SAFE_RELEASE_N(textureSamplerState);
	SAFE_RELEASE_N(cbPerObjectBuffer);
	SAFE_RELEASE_N(particleTexture);

}
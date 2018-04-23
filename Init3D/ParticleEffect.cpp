#include "ParticleEffect.h"

#define ONFAIL_RELEASE_RETURN(hr, a)	if (FAILED(hr)) { a.release(); return hr; }
#define SAFE_RELEASE(a)					if (a){ a->release(); delete a; a = nullptr; }
#define SAFE_RELEASE_N(a)				if (a){ a->Release(); a = nullptr; }

HRESULT ParticleEffect::createParticleEffect(ID3D11Device * d3d11Device, ParticleEffect & particleEffect)
{
	particleEffect.drawVS = new VertexShader;
	particleEffect.drawPS = new PixelShader;
	particleEffect.drawGS = new GeometryShader;

	HRESULT hr;
	hr = VertexShader::loadAndCreateVertexShader(L"VS_ParticleSystem.hlsl", d3d11Device, *particleEffect.drawVS);
	ONFAIL_RELEASE_RETURN(hr, particleEffect);
	hr = PixelShader::loadAndCreatePixelShader(L"PS_ParticleSystem.hlsl", d3d11Device, *particleEffect.drawPS);
	ONFAIL_RELEASE_RETURN(hr, particleEffect);
	hr = GeometryShader::loadAndCreateGeometryShader(L"GS_ParticleSystem.hlsl", d3d11Device, *particleEffect.drawGS);
	ONFAIL_RELEASE_RETURN(hr, particleEffect);

	particleEffect.sizeOfPerObjectCB = sizeof(cbPerObject);

	hr = d3d11Device->CreateInputLayout(ParticleVertex::layout, ParticleVertex::numElements,
		particleEffect.drawVS->Buffer->GetBufferPointer(), particleEffect.drawVS->Buffer->GetBufferSize(), &particleEffect.vertLayout);
	ONFAIL_RELEASE_RETURN(hr, particleEffect);

	return S_OK;
}

void ParticleEffect::applyDraw()
{
}

void ParticleEffect::release()
{
	SAFE_RELEASE(this->drawVS);
	SAFE_RELEASE(this->drawPS);
	SAFE_RELEASE(this->drawGS);

	SAFE_RELEASE_N(this->vertLayout);
}

ParticleEffect::ParticleEffect()
{
	this->drawVS = nullptr;
	this->drawPS = nullptr; 
	this->vertLayout = nullptr;
}


ParticleEffect::~ParticleEffect()
{
}

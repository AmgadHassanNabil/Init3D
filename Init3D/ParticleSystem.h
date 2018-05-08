#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertices.h"
#include "ParticleEffect.h"
#include "WICTextureLoader.h"

using namespace DirectX;

struct Dynamics
{
	XMFLOAT3 velocity;
	XMFLOAT2 endSize;
	float ttl;
	float particleWidth;
	float particleHeight;
	float spread;
};

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	inline void setEyePos(XMFLOAT3& eyePos){this->eyePos = eyePos;}

	HRESULT init(ID3D11Device* device, ParticleEffect* fx,
		UINT maxParticles,
		const wchar_t* texturePath, float spread);
	void release();

	void update(float dt, const XMFLOAT3& direction, const XMFLOAT3& emitPosition);
	void draw(ID3D11DeviceContext* dc, const XMMATRIX& VP, const XMFLOAT4& camPos, const XMFLOAT3& camUp);
private:
	ParticleSystem(const ParticleSystem& rhs);
	ParticleSystem& operator=(const ParticleSystem& rhs);

private:

	UINT mMaxParticles;

	float mGameTime;
	float mTimeStep;
	float mAge;

	XMFLOAT3 mEmitPosW;
	XMFLOAT3 mEmitDirW;
	XMFLOAT3 eyePos;

	ParticleEffect* mFX;
	ID3D11ShaderResourceView* particleTexture = NULL;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* instanceBuffer;

	ID3D11ShaderResourceView* mTexArraySRV;
	ID3D11ShaderResourceView* mRandomTexSRV;
	ID3D11SamplerState* textureSamplerState;
	ID3D11Buffer* cbPerObjectBuffer/*, *cbDynamicsBuffer*/;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	HRESULT buildVB(ID3D11Device* device, float spread);

	struct InstanceData
	{
		XMFLOAT3 pos;
		XMFLOAT4 random;
	};
};


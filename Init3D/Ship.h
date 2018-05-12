#pragma once
#include <DirectXMath.h>
#include <dinput.h>
#include "Input.h"
#include "Model.h"
#include "Shader.h"
#include "ParticleEffect.h"
#include "ParticleSystem.h"

using namespace DirectX;

class Ship
{

	const float RotationRate = 1.5f;
	const float Mass = 1.0f;
	const float ThrustForce = 24000.0f;
	const float DragFactor = 0.97f;

	float _302Accelaration = 0.001f;
	XMMATRIX world;

	bool motionStarted = false;
	float thrustAmount = 0;
	float turnTilt = 0;

	Model model;
	TexturedEffect * effect;
	ParticleEffect * particleEffect;
	ParticleSystem particleSystemLeft;
	ParticleSystem particleSystemRight;

	const XMVECTOR WorldUp = XMVectorSet(0, 1, 0, 0);
	const XMVECTOR WorldRight = XMVectorSet(1, 0, 0, 0);

	XMVECTOR leftThrusterOffset = XMVectorSet(11.8, -2.75, 2.5, 0);
	XMVECTOR rightThrusterOffset = XMVectorSet(-11.8, -2.75, 2.5, 0);
	int leftThrusterAnimation = 30;
	int rightThrusterAnimation = 30;

	void calculateAxes(XMVECTOR & direction, XMVECTOR & upNoTilt, XMVECTOR & up, XMVECTOR & right, XMVECTOR & rightNoTilt,
						const XMMATRIX & xyRotationMatrix, const XMMATRIX &tiltMatrix, XMMATRIX& xyzRotation);
public:
	XMVECTOR position, up, upNoTilt, direction, right, rightNoTilt, velocity;

	Ship();
	~Ship();
	HRESULT initialize(const char* shipFilePath, const wchar_t* texturePath, TexturedEffect *effect, ParticleEffect* particleEffect);
	void update(const double& time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[]);
	void getWorld(XMMATRIX& world) { world = this->world; }
	void draw(const XMMATRIX& viewXProjection, const XMFLOAT4& fCamPos, XMFLOAT3& fCamUp);
	void release();
};


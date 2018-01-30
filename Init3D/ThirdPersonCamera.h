#pragma once
#include <DirectXMath.h>
#include <dinput.h>
#include "Input.h"

using namespace DirectX;

class ThirdPersonCamera
{
	XMMATRIX view;

	XMVECTOR chasePosition;
	XMVECTOR chaseDirection;
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR desiredPositionOffset = XMVectorSet(0, 5, 20, 1);
	XMVECTOR desiredPosition;
	XMVECTOR lookAtOffset = XMVectorSet(0, 5, 0, 1);
	XMVECTOR lookAt;

	float stiffness = 1800.0f;
	float damping = 600.0f;
	float mass = 50.0f;

	XMVECTOR position;
	XMVECTOR velocity;

	void updateWorldPositions();
	void updateMatrices();

public:
	void reset();
	ThirdPersonCamera();
	void update(const double & time, const XMVECTOR& referencePosition, const XMVECTOR& chaseDirection, const XMVECTOR& up);
	inline void getView(XMMATRIX &view) { view = this->view; }
	~ThirdPersonCamera();
};


#pragma once
#include <DirectXMath.h>

#include "Input.h"
#include "Defines.h"

using namespace DirectX;

class ThirdPersonCamera
{
	XMMATRIX view;

	XMVECTOR chasePosition;
	XMVECTOR chaseDirection;
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR desiredPositionOffset;
	XMVECTOR desiredPosition;
	XMVECTOR lookAtOffset;
	XMVECTOR lookAt;

	float stiffness = 1800.0f;
	float damping = 300.0f;
	float mass = 50.0f;
	float cameraRotationAcceleration = 0.005f;

	float xRotation, yRotation;

	XMVECTOR position;
	XMVECTOR velocity;

	void updateWorldPositions();
	void updateMatrices();

public:
	void reset();
	void handleMouseInput(const double & time, const DIMOUSESTATE & mouseCurrState);
	ThirdPersonCamera();
	void update(const double & time, const DIMOUSESTATE & mouseCurrState, const XMVECTOR& referencePosition, const XMVECTOR& chaseDirection, const XMVECTOR& up);
	inline void getView(XMMATRIX &view) { view = this->view; }
	inline void getPosition(XMVECTOR& position) { position = this->position; }
	inline void getLookAt(XMVECTOR& lookAt) { lookAt = this->lookAt; }
	inline void getUp(XMVECTOR& up) { up = this->up; }
	~ThirdPersonCamera();
};


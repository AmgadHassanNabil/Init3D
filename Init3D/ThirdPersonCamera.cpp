#include "ThirdPersonCamera.h"


ThirdPersonCamera::ThirdPersonCamera()
{
	velocity = XMVectorSet(0, 0, 0, 0);
	lookAtOffset = XMVectorSet(0, 5, 30, 0);
	desiredPositionOffset = XMVectorSet(0, 5, 70, 0);
	xRotation = yRotation = 0;
}

void ThirdPersonCamera::updateWorldPositions()
{
	XMVECTOR right = XMVector3Cross(up, chaseDirection);

	XMMATRIX transform = TRANSFORMATION_MATRIX(chaseDirection, up, right, XMVectorZero());

	XMVECTOR desiredPositionOffsetTransformed = XMVector3Transform(desiredPositionOffset, XMMatrixRotationRollPitchYaw(xRotation, yRotation, 0));

	desiredPosition = chasePosition + XMVector3Transform(desiredPositionOffsetTransformed, transform);

	lookAt = chasePosition + XMVector3TransformNormal(lookAtOffset, transform);
}

void ThirdPersonCamera::updateMatrices()
{
	float w = XMVectorGetW(position);
	float x = XMVectorGetX(position);
	float y = XMVectorGetY(position);
	float z = XMVectorGetZ(position);
	XMVECTOR positionHomogenous = XMVectorSet(x / w, y / w, z / w, 1);

	view = XMMatrixLookAtLH(positionHomogenous, this->lookAt, this->up);
}

void ThirdPersonCamera::reset()
{
	updateWorldPositions();
	xRotation = yRotation = 0;
	// Stop motion
	velocity = XMVectorZero();

	// Force desired position
	position = desiredPosition;

	updateMatrices();
}

void ThirdPersonCamera::handleMouseInput(const double & time, const DIMOUSESTATE & mouseCurrState)
{
	DIMOUSESTATE mouseLastState;
	AMINPUT->getLastMouseState(mouseLastState);

	if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
	{
		yRotation += mouseLastState.lX * cameraRotationAcceleration;

		xRotation += -mouseCurrState.lY * cameraRotationAcceleration;
	}
}

void ThirdPersonCamera::update(const double & time, const DIMOUSESTATE & mouseCurrState, const XMVECTOR & chasePosition, const XMVECTOR & chaseDirection, const XMVECTOR & up)
{
	this->chasePosition = chasePosition;
	this->chaseDirection = chaseDirection;
	this->up = up;

	handleMouseInput(time, mouseCurrState);
	

	updateWorldPositions();

	// Calculate spring force
	XMVECTOR stretch = position - desiredPosition;
	XMVECTOR force = -stiffness * stretch - damping * velocity;

	// Apply acceleration
	XMVECTOR acceleration = force / mass;
	velocity += acceleration * time;

	// Apply velocity
	position += velocity * time;

	updateMatrices();
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

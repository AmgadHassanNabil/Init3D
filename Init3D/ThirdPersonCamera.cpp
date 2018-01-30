#include "ThirdPersonCamera.h"


ThirdPersonCamera::ThirdPersonCamera()
{
	velocity = XMVectorSet(0, 0, 0, 1);
}

void ThirdPersonCamera::updateWorldPositions()
{
	XMFLOAT3 pos, desPos, desPosOff, dir, chasePos;
	XMStoreFloat3(&pos, position);
	XMStoreFloat3(&desPos, desiredPosition);
	XMStoreFloat3(&desPosOff, desiredPositionOffset);
	XMStoreFloat3(&dir, chaseDirection);
	XMStoreFloat3(&chasePos, chasePosition);

	XMVECTOR right = XMVector3Cross(up, chaseDirection);
	
	XMMATRIX transform = TRANSFORMATION_MATRIX(chaseDirection, up, right, XMVectorZero());

	desiredPosition = chasePosition + XMVector3Transform (	desiredPositionOffset, transform);
	XMStoreFloat3(&desPos, desiredPosition);
	lookAt = chasePosition + XMVector3TransformNormal (lookAtOffset, transform);
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

	// Stop motion
	velocity = XMVectorZero();

	// Force desired position
	position = desiredPosition;

	XMFLOAT3 pos, desPos, desPosOff;
	XMStoreFloat3(&pos, position);
	XMStoreFloat3(&desPos, desiredPosition);
	XMStoreFloat3(&desPosOff, desiredPositionOffset);
	updateMatrices();
}


void ThirdPersonCamera::update(const double & time, const XMVECTOR & chasePosition, const XMVECTOR & chaseDirection, const XMVECTOR & up)
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, position);

	this->chasePosition = chasePosition;
	this->chaseDirection = chaseDirection;
	this->up = up;

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

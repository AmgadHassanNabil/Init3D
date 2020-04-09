#include "Ship.h"
#include "Logger.h"

//Logger logger;

#define ONFAIL_RELEASE_RETURN(hr, a)	if (FAILED(hr)) { a->release(); return hr; }
#define SAFE_RELEASE(a)					if (a){ a.release(); }


void creationCriteria(int * p_numberOfParticles, float * p_spread, float mAge)
{
	float threshold = 0.07f * (mAge * mAge * mAge * mAge);
	(*p_numberOfParticles) = 20 + (3 * threshold);
	(*p_spread) = 0.1 * threshold;
}

HRESULT Ship::initialize(const char* shipFilePath, const char* missleFilePath, const wchar_t* thrusterParticleTexturePath, TexturedEffect *effect, ParticleEffect* particleEffect)
{
	this->effect = effect;
	HRESULT hr;

	hr = particleSystemLeft.init(AMD3D->d3d11Device, particleEffect, 50, thrusterParticleTexturePath, 1, NULL);
	ONFAIL_RELEASE_RETURN(hr, this);
	hr = particleSystemRight.init(AMD3D->d3d11Device, particleEffect, 50, thrusterParticleTexturePath, 1, NULL);
	ONFAIL_RELEASE_RETURN(hr, this);
	hr = particleSystemMissle.init(AMD3D->d3d11Device, particleEffect, 50, thrusterParticleTexturePath, 1, NULL);
	ONFAIL_RELEASE_RETURN(hr, this);
	hr = particleSystemMain.init(AMD3D->d3d11Device, particleEffect, 100, thrusterParticleTexturePath, 1, creationCriteria);
	ONFAIL_RELEASE_RETURN(hr, this);
	hr = model.loadFromFile(shipFilePath, AMD3D->d3d11Device, effect);
	ONFAIL_RELEASE_RETURN(hr, this);
	hr = missle.loadFromFile(missleFilePath, AMD3D->d3d11Device, effect);
	ONFAIL_RELEASE_RETURN(hr, this);
	return S_OK;
}


float missleSpeed = 0.5;
float missleThreshold = 0;

void Ship::update(const double & time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[])
{
	BYTE *lastKeyboardState;
	AMINPUT->getLastKeyboardState(&lastKeyboardState);

	XMFLOAT2 rotationAmount(0, 0);
	turnTilt = 0;
	
	if (INPUT_DOWN(currKeyboardState[DIK_A]))
	{
		rotationAmount.x = -1.0f;
		turnTilt = -0.008f;
	}
	if (INPUT_DOWN(currKeyboardState[DIK_D]))
	{
		rotationAmount.x = 1.0f;
		turnTilt = 0.008f;
	}
	if (INPUT_DOWN(currKeyboardState[DIK_W]))
		rotationAmount.y = 1.0f;
	if (INPUT_DOWN(currKeyboardState[DIK_S]))
		rotationAmount.y = -1.0f;
	

	float turningSpeed = time * 1.5f;
	rotationAmount.x *= turningSpeed;
	rotationAmount.y *= turningSpeed;

	if (XMVectorGetY(up) < 0)
		rotationAmount.x = -rotationAmount.x;

	float dot = XMVectorGetY(XMVector3Dot(up, upNoTilt));
	if (dot < 0.4)
		turnTilt = 0;

	//Obtain the angle between current Up vector, and no Tilt Up vector then linearly interpolate to this rotation
	if (rotationAmount.x == 0)
	{
		float upLength = XMVectorGetY(XMVector3Length(up));
		float rotationToAngle = XMScalarACos((dot) / upLength);

		float rightDot = XMVectorGetY(XMVector3Dot(up, rightNoTilt));
		turnTilt = VALUE_IF_IN_THREASHOLD((rightDot < 0 ? rotationToAngle : -rotationToAngle) * 0.002f, 0.0001f, 0);
	}

	XMMATRIX rotationMatrix =
		XMMatrixRotationAxis(right, rotationAmount.y)
		*
		XMMatrixRotationY(rotationAmount.x);
	XMMATRIX rotationMatrixWithTilt;
	calculateAxes(direction, upNoTilt, up, right, rightNoTilt, rotationMatrix, XMMatrixRotationAxis(direction, turnTilt), rotationMatrixWithTilt);

	if (motionStarted)
		thrustAmount = clamp(0.03, 0.3, thrustAmount);

	if (INPUT_DOWN(currKeyboardState[DIK_SPACE]))
	{
		thrustAmount += 0.0005;
		motionStarted = mainThrusterEngaged = true;
		if(INPUT_UP(lastKeyboardState[DIK_SPACE]))
			particleSystemMain.reset();
	}
	else
	{
		mainThrusterEngaged = false;
		if (motionStarted)
			thrustAmount -= 0.0004f;
	}

	
	XMVECTOR force = direction * thrustAmount * ThrustForce;

	XMVECTOR acceleration = force / Mass;
	velocity += acceleration * time;

	velocity *= DragFactor;
	XMFLOAT3 vel;
	XMStoreFloat3(&vel, velocity);
	position += (-velocity) * time;

	XMMATRIX transform = TRANSFORMATION_MATRIX(direction, up, right, position);

	world = XMMatrixScaling(0.05f, 0.05f, 0.05f) * transform;

	if (INPUT_DOWN(currKeyboardState[DIK_R]) && INPUT_UP(lastKeyboardState[DIK_R]))
	{
		missleFired = true;
		if(targetIndex != -1)
			missleThrusterDirection = -XMVector3NormalizeEst(position - targetPosition);
		else
			missleThrusterDirection = -direction;
		missleSpeed = 0.0;
		missleThrusterPosition = position + missleThrusterOffset;
		missleThreshold = 0;
	}

	//Update thrusters
	if (rotationAmount.x > 0)
	{
		if (leftThrusterAnimation < 60)
			leftThrusterAnimation++;
	}
	else
	{
		if (leftThrusterAnimation > 30)
			leftThrusterAnimation--;
	}
	if (rotationAmount.x < 0)
	{
		if (rightThrusterAnimation < 60)
			rightThrusterAnimation++;
	}
	else
	{
		if (rightThrusterAnimation > 30)
			rightThrusterAnimation--;
	}	

	XMFLOAT3 leftThrusterPosition, rightThrusterPosition, mainThrusterPosition, leftThrusterDirection, rightThrusterDirection, mainThrusterDirection, missleThrusterOffsetF;

	XMStoreFloat3(&leftThrusterDirection, direction * leftThrusterAnimation);
	leftThrusterOffset = XMVector3Transform(leftThrusterOffset, rotationMatrixWithTilt);
	XMStoreFloat3(&leftThrusterPosition, position + leftThrusterOffset);
	particleSystemLeft.update(time, leftThrusterDirection, leftThrusterPosition);

	rightThrusterOffset = XMVector3Transform(rightThrusterOffset, rotationMatrixWithTilt);
	XMStoreFloat3(&rightThrusterPosition, position + rightThrusterOffset);
	XMStoreFloat3(&rightThrusterDirection, direction * rightThrusterAnimation);
	particleSystemRight.update(time, rightThrusterDirection, rightThrusterPosition);

	mainThrusterOffset = XMVector3Transform(mainThrusterOffset, rotationMatrixWithTilt);
	XMStoreFloat3(&mainThrusterPosition, position + mainThrusterOffset);
	XMStoreFloat3(&mainThrusterDirection, direction * 40);
	if (mainThrusterEngaged)
		particleSystemMain.update(time, mainThrusterDirection, mainThrusterPosition);
	
	XMStoreFloat3(&missleThrusterOffsetF, missleThrusterPosition - (missleThrusterDirection * 11));
	if (missleFired)
	{
		missleThreshold += 0.5f * time;
		missleThreshold = min(missleThreshold, 1);
		XMVECTOR missleDirection = ((missleThrusterDirection * missleThreshold) + (-direction * (1 - missleThreshold)));

		missleThrusterPosition += missleDirection * (missleSpeed += (1.2 * time));
		XMFLOAT3 missleThrusterDirectionF;
		XMStoreFloat3(&missleThrusterDirectionF, -missleDirection * 80);
		particleSystemMissle.update(time, missleThrusterDirectionF, missleThrusterOffsetF);
		missleWorld = TRANSFORMATION_MATRIX(missleDirection, WorldUp, XMVector3Cross(WorldUp, missleDirection), missleThrusterPosition);
	}
	else
	{
		missleThrusterOffset = XMVector3Transform(missleThrusterOffset, rotationMatrixWithTilt);
		missleThrusterPosition = position + missleThrusterOffset;
		missleWorld = TRANSFORMATION_MATRIX(-direction, WorldUp, XMVector3Cross(WorldUp, missleThrusterDirection), missleThrusterPosition);
	}
}

void Ship::setTarget(const XMFLOAT3 & targetPosition, const int & index)
{
	this->targetPosition=XMLoadFloat3(&targetPosition);
	this->targetIndex = index;
}

void Ship::draw(const XMMATRIX& viewXProjection, const XMFLOAT4& fCamPos, XMFLOAT3& fCamUp)
{
	
	XMMATRIX WVP;

	WVP = XMMatrixMultiply(scaleMatrix * missleWorld, viewXProjection);
	
	effect->cbPerObj.WVP = XMMatrixTranspose(WVP);
	effect->cbPerObj.World = XMMatrixTranspose(world);
	missle.draw(AMD3D->d3d11DevCon);


	WVP = XMMatrixMultiply(world, viewXProjection);

	effect->cbPerObj.WVP = XMMatrixTranspose(WVP);
	effect->cbPerObj.World = XMMatrixTranspose(world);

	model.draw(AMD3D->d3d11DevCon);

	AMD3D->enableAdditiveBlending();
	AMD3D->noDepthWrite();
	particleSystemLeft.draw(AMD3D->d3d11DevCon, viewXProjection, fCamPos, fCamUp);
	particleSystemRight.draw(AMD3D->d3d11DevCon, viewXProjection, fCamPos, fCamUp);
	if (mainThrusterEngaged)
		particleSystemMain.draw(AMD3D->d3d11DevCon, viewXProjection, fCamPos, fCamUp);
	if (missleFired)
		particleSystemMissle.draw(AMD3D->d3d11DevCon, viewXProjection, fCamPos, fCamUp);
	AMD3D->enableDefaultBlending();
	AMD3D->defaultDepth();
}

void Ship::calculateAxes(XMVECTOR & direction, XMVECTOR & upNoTilt, XMVECTOR & up, XMVECTOR & right, XMVECTOR & rightNoTilt, const XMMATRIX & xyRotationMatrix, const XMMATRIX & tiltMatrix, XMMATRIX& xyzRotation)
{
	direction = XMVector3TransformNormal(direction, xyRotationMatrix);
	upNoTilt = XMVector3TransformNormal(upNoTilt, xyRotationMatrix);
	xyzRotation = tiltMatrix * xyRotationMatrix;
	up = XMVector3TransformNormal(up, xyzRotation);
	direction = XMVector3Normalize(direction);
	up = XMVector3Normalize(up);
	upNoTilt = XMVector3Normalize(upNoTilt);

	right = XMVector3Cross(direction, up);
	rightNoTilt = XMVector3Cross(direction, upNoTilt);

	up = XMVector3Cross(right, direction);
	upNoTilt = XMVector3Cross(rightNoTilt, direction);
}

void Ship::release()
{
	particleSystemLeft.release();
	particleSystemRight.release();
	model.release();
}

Ship::Ship()
{
	position = XMVectorSet(30, 0, -40, 0);//XMVectorZero();
	direction = XMVectorSet(0, 0, 1, 1);
	up = upNoTilt = XMVectorSet(0, 1, 0, 1);
	right = XMVectorSet(1, 0, 0, 1);
	velocity = XMVectorZero();
}
Ship::~Ship()
{
}

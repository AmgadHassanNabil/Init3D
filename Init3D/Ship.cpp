#include "Ship.h"
#include "Logger.h"

Logger logger;

HRESULT Ship::initialize(const char* shipFilePath, TexturedEffect *effect)
{
	this->effect = effect;
	return model.loadFromFile(shipFilePath, AMD3D->d3d11Device, effect);
}
void Ship::draw(const XMMATRIX& viewXProjection)
{
#ifdef _WIN64
	XMMATRIX WVP = XMMatrixMultiply(world, viewXProjection);
#else
	WVP = World * camView * camProjection
#endif
	effect->cbPerObj.WVP = XMMatrixTranspose(WVP);
	effect->cbPerObj.World = XMMatrixTranspose(world);


	model.draw(AMD3D->d3d11DevCon);
}

void Ship::update(const double & time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[])
{
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

	calculateAxes(direction, upNoTilt, up, right, rightNoTilt, rotationMatrix, XMMatrixRotationAxis(direction, turnTilt));

	if (motionStarted)
		thrustAmount = clamp(0.03, 0.5, thrustAmount);

	if (INPUT_DOWN(currKeyboardState[DIK_SPACE]))
	{
		thrustAmount += 0.005;
		motionStarted = true;
	}
	else if (motionStarted)
		thrustAmount -= 0.0004f;

	
	XMVECTOR force = direction * thrustAmount * ThrustForce;

	XMVECTOR acceleration = force / Mass;
	velocity += acceleration * time;

	velocity *= DragFactor;
	XMFLOAT3 vel;
	XMStoreFloat3(&vel, velocity);
	position += (-velocity) * time;

	XMMATRIX transform = TRANSFORMATION_MATRIX(direction, up, right, position);

	world = XMMatrixScaling(0.05f, 0.05f, 0.05f) * transform;
}

void Ship::calculateAxes(XMVECTOR & direction, XMVECTOR & upNoTilt, XMVECTOR & up, XMVECTOR & right, XMVECTOR & rightNoTilt, const XMMATRIX & xyRotationMatrix, const XMMATRIX & tiltMatrix)
{
	direction = XMVector3TransformNormal(direction, xyRotationMatrix);
	upNoTilt = XMVector3TransformNormal(upNoTilt, xyRotationMatrix);
	up = XMVector3TransformNormal(up, tiltMatrix * xyRotationMatrix);
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

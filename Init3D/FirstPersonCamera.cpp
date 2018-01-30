#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera()
{
	camPosition = XMVectorSet(0, 0, 0, 0);
	xRotation = yRotation = 0;
}

void FirstPersonCamera::detectInput(const double & time, DIMOUSESTATE mouseCurrState, const BYTE currKeyboardState[])
{
	if ((currKeyboardState[DIK_W] & 0x80))
		moveBackForward += time * moveAcceleration;
	if ((currKeyboardState[DIK_S] & 0x80))
		moveBackForward -= time * moveAcceleration;
	if ((currKeyboardState[DIK_A] & 0x80))
		moveLeftRight -= time * moveAcceleration;
	if ((currKeyboardState[DIK_D] & 0x80))
		moveLeftRight += time * moveAcceleration;

	DIMOUSESTATE mouseLastState;
	AMINPUT->getLastMouseState(mouseLastState);

	if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
	{
		yRotation += mouseLastState.lX * cameraRotationAcceleration;

		xRotation += mouseCurrState.lY * cameraRotationAcceleration;

		mouseLastState = mouseCurrState;
	}
}
void FirstPersonCamera::update(const double & time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[])
{
	detectInput(time, mouseCurrState, currKeyboardState);
	camRotationMatrix = XMMatrixRotationRollPitchYaw(xRotation, yRotation, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camUp = XMVector3Cross(camForward, camRight);

	camPosition += moveLeftRight * camRight;
	camPosition += moveBackForward * camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

FirstPersonCamera::~FirstPersonCamera()
{
}

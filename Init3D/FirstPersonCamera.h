#pragma once
//#include"Headers.h"
#include <DirectXMath.h>
#include <dinput.h>
#include "Input.h"

using namespace DirectX;

class FirstPersonCamera
{
	XMMATRIX camView;
	
	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	const XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX camRotationMatrix;

	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
	float xRotation = 0.0f;
	float yRotation = 0.0f;


	
	FirstPersonCamera(const FirstPersonCamera&);
	FirstPersonCamera& operator=(const FirstPersonCamera&);
	

	void detectInput(const double & time, DIMOUSESTATE mouseCurrState, const BYTE currKeyboardState[]);

public:
	float moveAcceleration = 15;
	float cameraRotationAcceleration = 0.005f;


	FirstPersonCamera();
	~FirstPersonCamera();
	void update(const double& time, DIMOUSESTATE mouseCurrState, BYTE currKeyboardState[]);
	inline void getView(XMMATRIX &view) { view = this->camView; }

};

